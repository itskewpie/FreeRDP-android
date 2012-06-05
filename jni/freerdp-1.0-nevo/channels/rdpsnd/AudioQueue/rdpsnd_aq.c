/**
 * FreeRDP: A Remote Desktop Protocol client.
 * Audio Output Virtual Channel AudioQueue backend
 *
 * Copyright 2012 Nevo (snowgeekx@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rdpsnd_aq.h"


#define kNumberBuffers 3


static void rdpsnd_aq_close(rdpsndDevicePlugin* device);
static void rdpsnd_aq_open(rdpsndDevicePlugin* device,
                           rdpsndFormat* format,
                           int latency);


static void rdpsnd_aq_handle_output(void *aqData,
                                    AudioQueueRef inAQ,
                                    AudioQueueBufferRef inBuffer)
{
    int len;
    OSStatus status;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)aqData;

    len = CFArrayGetCount(aq->mPendingBlocks);

    if (len > 0) {
        CFDataRef block =
            (CFDataRef)CFArrayGetValueAtIndex(aq->mPendingBlocks, 0);
        inBuffer->mAudioDataByteSize = CFDataGetLength(block);
        memcpy(inBuffer->mAudioData, CFDataGetBytePtr(block),
               inBuffer->mAudioDataByteSize);
        status = AudioQueueEnqueueBuffer(aq->mQueue, inBuffer, 0, NULL);
        pthread_mutex_lock(&aq->mBufLock);
        CFArrayRemoveValueAtIndex(aq->mPendingBlocks, 0);
        pthread_mutex_unlock(&aq->mBufLock);
    } else {
        // Mark buffer to be available for enqueuing
        int i;
        inBuffer->mAudioDataByteSize = 0;
        for (i = 0; i < aq->mNumBuffers; i++) {
            if (aq->mBuffers[i].buf == inBuffer) {
                pthread_mutex_lock(&aq->mBufLock);
                aq->mBuffers[i].enqueued = FALSE;
                pthread_mutex_unlock(&aq->mBufLock);
                break;
            }
        }
    }
}


static void rdpsnd_aq_is_running(void *inUserData,
                                 AudioQueueRef inAQ,
                                 AudioQueuePropertyID inID)
{
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)inUserData;
    UInt32 isRunning = 0;
    UInt32 size = sizeof(isRunning);
    OSStatus result = AudioQueueGetProperty(inAQ, kAudioQueueProperty_IsRunning, &isRunning, &size);

    aq->mIsRunning = isRunning;

    if ((result == noErr) && (!isRunning)) {
        printf("%s: AudioQueue has been stopped\n", __func__);
    }
}


static void rdpsnd_aq_derive_buffersize (AudioStreamBasicDescription ASBDesc,
                                         uint32 maxPacketSize,
                                         double seconds,
                                         uint32 *outBufferSize,
                                         uint32 *outNumPacketsToRead)
{
    static const int maxBufferSize = 0x10000;
    static const int minBufferSize = 0x4000;

    if (ASBDesc.mFramesPerPacket != 0) {
        Float64 numPacketsForTime =
            ASBDesc.mSampleRate / ASBDesc.mFramesPerPacket * seconds;
        *outBufferSize = numPacketsForTime * maxPacketSize;
    } else {
        *outBufferSize =
            maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
    }

    if (*outBufferSize > maxBufferSize &&
        *outBufferSize > maxPacketSize ) {
        *outBufferSize = maxBufferSize;
    } else if (*outBufferSize < minBufferSize) {
        *outBufferSize = minBufferSize;
    }
    *outNumPacketsToRead = maxPacketSize ? *outBufferSize / maxPacketSize : 0;
}


static boolean rdpsnd_aq_format_supported(rdpsndDevicePlugin* device,
                                          rdpsndFormat* format)
{
    switch (format->wFormatTag) {
    case 1: /* PCM */
        if (format->cbSize == 0 &&
            format->nSamplesPerSec <= 48000 &&
            (format->wBitsPerSample == 8 || format->wBitsPerSample == 16) &&
            (format->nChannels == 1 || format->nChannels == 2)) {
            return true;
        }
        break;

    case 0x11: /* IMA ADPCM */
        if (format->nSamplesPerSec <= 48000 &&
            format->wBitsPerSample == 4 &&
            (format->nChannels == 1 || format->nChannels == 2)) {
            return true;
        }
        break;
    }
    return false;
}


static void rdpsnd_aq_set_volume(rdpsndDevicePlugin* device,
                                 uint32 value)
{
    OSStatus status;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;
    status = AudioQueueSetParameter(aq->mQueue, kAudioQueueParam_Volume, value);
    if (status != 0) {
        printf("%s [warning]: failed to set volume, returned status %d\n",
               __func__, (int)status);
    }
}


static void rdpsnd_aq_set_format(rdpsndDevicePlugin* device,
                                 rdpsndFormat* format,
                                 int latency)
{
    uint32 maxpacketsize = 0;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;

    if (format != NULL) {
        bzero(&aq->mDataFormat, sizeof aq->mDataFormat);

        aq->mDataFormat.mSampleRate = format->nSamplesPerSec;
        aq->mDataFormat.mChannelsPerFrame = format->nChannels;

        /*
         * AppleIMA4 format is different from Microsoft ADPCM, although
         * they are both encoded in ADPCM algorithm.
         * See http://www.fenestrated.net/~macman/mirrors/Apple%20Technotes%20(As%20of%202002)/tn/tn1081.html
         */

        switch (format->wFormatTag) {
        case 1: /* PCM */
        case 0x11: /* IMA ADPCM */
            /*
             * ADPCM will be decoded into PCM due to the difference
             * between IMA4 and ADPCM.
             */
            aq->mDataFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger |
                                           kAudioFormatFlagIsPacked;
            aq->mDataFormat.mBytesPerPacket =
                aq->mDataFormat.mBytesPerFrame = format->nChannels * 2;
            aq->mDataFormat.mFramesPerPacket = 1;
            aq->mDataFormat.mBitsPerChannel = 16;
            aq->mDataFormat.mFormatID = kAudioFormatLinearPCM;
            break;
        default:
            printf("%s [warning]: __not__reached__\n", __func__);
            break;
        }
        aq->mFormat = format->wFormatTag;
        aq->mBlockSize = format->nBlockAlign;
        //aq->mDataFormat.mBytesPerFrame = aq->block_size;
        printf("%s: sample rate %d, %d channels, bits per sample %d, 0x%X, "
               "block size %d, latency %d\n",
               __func__, format->nSamplesPerSec, format->nChannels,
               format->wBitsPerSample, format->wFormatTag, aq->mBlockSize,
               latency);

        aq->mLatency = latency;

        bzero(&aq->mAdpcm, sizeof aq->mAdpcm);
        // Compute buffer size for CBR (VBR is not supported)
        maxpacketsize = aq->mDataFormat.mBytesPerPacket;
        rdpsnd_aq_derive_buffersize(aq->mDataFormat, maxpacketsize,
                                    0.08, &aq->mBufferByteSize,
                                    &aq->mNumPacketsToRead);

        /*
         * XXX Not sure why set_format will be called occasionally
         *     during playback, it looks the only way to alter
         *     audio stream format for audio queue when it's running
         *     is reset audio queue, re-enqueue buffers again.
         *     Using Audio Unit might look be a future option for
         *     addressing this situation.
         */
        if (aq->mIsRunning) {
            printf("%s [warning]: resetting AudioQueue during playback "
                   "is likely to cause audio discontinuity, you may "
                   "consider avoid playing audio with variable properties.\n",
                   __func__);
            rdpsnd_aq_close(device);
            rdpsnd_aq_open(device, format, latency);
        }
    }
}


static void rdpsnd_aq_open(rdpsndDevicePlugin* device,
                           rdpsndFormat* format,
                           int latency)
{
    int i;
    OSStatus ret;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;

    rdpsnd_aq_set_format(device, format, latency);
    ret = AudioQueueNewOutput(&aq->mDataFormat, rdpsnd_aq_handle_output,
                              aq, NULL, kCFRunLoopCommonModes,
                              0, &aq->mQueue);
    printf("%s: create audio queue %p %d\n", __func__, aq->mQueue, (int)ret);

    AudioQueueAddPropertyListener(aq->mQueue, kAudioQueueProperty_IsRunning,
                                  rdpsnd_aq_is_running, aq);

    // Initialize buffer queue
    aq->mNumBuffers = kNumberBuffers;
    aq->mBuffers = malloc(aq->mNumBuffers * sizeof(rdpsndAudioQueueBuffer));
    for (i = 0; i < aq->mNumBuffers; i++) {
        ret = AudioQueueAllocateBuffer(aq->mQueue, aq->mBufferByteSize,
                                       &aq->mBuffers[i].buf);
        aq->mBuffers[i].enqueued = FALSE;
    }
}


static void rdpsnd_aq_close(rdpsndDevicePlugin* device)
{
    OSStatus status;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;

    status = AudioQueueStop(aq->mQueue, FALSE);

    printf("%s: close and dispose audio queue %d\n", __func__, (int)status);
    AudioQueueDispose(aq->mQueue, 0);
    aq->mQueue = NULL;
    free(aq->mBuffers);
    aq->mBuffers = NULL;

    aq->mIsRunning = FALSE;
}


static void rdpsnd_aq_free(rdpsndDevicePlugin* device)
{
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;
    printf("%s: quit AudioQueue plugin\n", __func__);

    rdpsnd_aq_close(device);

    CFRelease(aq->mPendingBlocks);
    aq->mPendingBlocks = NULL;
    pthread_mutex_destroy(&aq->mBufLock);

    xfree(device);
}


static void rdpsnd_aq_start(rdpsndDevicePlugin* device)
{
    OSStatus status;
    AudioTimeStamp startTime = { 0 };
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;

    if (!aq->mIsRunning) {
        startTime.mSampleTime =
            aq->mLatency * aq->mDataFormat.mSampleRate / 1000;
        status = AudioQueueSetParameter(aq->mQueue, kAudioQueueParam_Volume, 1.0);
        status = AudioQueuePrime(aq->mQueue, 0, NULL);
        status = AudioQueueStart(aq->mQueue, aq->mLatency > 0 ? &startTime : NULL);
        printf("%s: start AudioQueue [%d] %d\n", __func__, aq->mBufferByteSize,
               (int)status);
        if (status == 0) {
            /*
             * Set running status here although it will get set from property
             * listener.
             */
            aq->mIsRunning = TRUE;
        }
    }

}


static void rdpsnd_aq_play(rdpsndDevicePlugin* device,
                           uint8* data,
                           int size)
{
    int i;
    OSStatus status;
    rdpsndAudioQueuePlugin *aq = (rdpsndAudioQueuePlugin *)device;

    uint8 *decoded_data;
    int decoded_size;
    uint8 *src;

    CFDataRef block;

    if (!aq->mQueue) {
        printf("%s [warning]: queue is not created yet\n", __func__);
        return;
    }

    if (aq->mFormat == 0x11) {
        decoded_data = dsp_decode_ima_adpcm(&aq->mAdpcm, data, size,
                                            aq->mDataFormat.mChannelsPerFrame,
                                            aq->mBlockSize, &decoded_size);
        size = decoded_size;
        src = decoded_data;
    } else {
        decoded_data = NULL;
        src = data;
    }

    pthread_mutex_lock(&aq->mBufLock);

    for (i = 0; i < aq->mNumBuffers; i++) {
        if (!aq->mBuffers[i].enqueued) {
            if (aq->mBufferByteSize >= size) {
                aq->mBuffers[i].buf->mAudioDataByteSize = size;
                memcpy(aq->mBuffers[i].buf->mAudioData,
                       src, size);
            } else {
                printf("%s [warning]: the size of data to play is larger than "
                       "the capacity the audio queue can support per buffer. "
                       "you can try to increase the audio queue buffer size or "
                       "the data will be truncated to fit in one buffer\n",
                       __func__);
                aq->mBuffers[i].buf->mAudioDataByteSize = aq->mBufferByteSize;
                memcpy(aq->mBuffers[i].buf->mAudioData, src,
                       aq->mBufferByteSize);
            }
            status = AudioQueueEnqueueBuffer(aq->mQueue, aq->mBuffers[i].buf,
                                             0, NULL);
            aq->mBuffers[i].enqueued = TRUE;
            pthread_mutex_unlock(&aq->mBufLock);
            goto ret;
        }
    }
    pthread_mutex_unlock(&aq->mBufLock);

    rdpsnd_aq_start(device);

    // Add the data to pending list if no buffer is available for now
    block = CFDataCreate(NULL, src, size);
    pthread_mutex_lock(&aq->mBufLock);
    CFArrayAppendValue(aq->mPendingBlocks, block);
    pthread_mutex_unlock(&aq->mBufLock);
    CFRelease(block);

 ret:

    if (decoded_data) {
        xfree(decoded_data);
    }
}


int rdpsnd_aq_FreeRDPRdpsndDeviceEntry(PFREERDP_RDPSND_DEVICE_ENTRY_POINTS pEntryPoints)
{
    rdpsndAudioQueuePlugin *aq;

    printf("%s: load AudioQueue plugin\n", __func__);

    aq = xnew(rdpsndAudioQueuePlugin);
    bzero(aq, sizeof(rdpsndAudioQueuePlugin));

    aq->mDevice.Open = rdpsnd_aq_open;
    aq->mDevice.FormatSupported = rdpsnd_aq_format_supported;
    aq->mDevice.SetFormat = rdpsnd_aq_set_format;
    aq->mDevice.SetVolume = rdpsnd_aq_set_volume;
    aq->mDevice.Play = rdpsnd_aq_play;
    aq->mDevice.Start = rdpsnd_aq_start;
    aq->mDevice.Close = rdpsnd_aq_close;
    aq->mDevice.Free = rdpsnd_aq_free;

    pEntryPoints->pRegisterRdpsndDevice(pEntryPoints->rdpsnd,
                                        (rdpsndDevicePlugin *)aq);

    // Create array to keep pending data blocks
    aq->mPendingBlocks = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);

    // Create shared data read/write lock
    pthread_mutex_init(&aq->mBufLock, NULL);

    return 0;
}
