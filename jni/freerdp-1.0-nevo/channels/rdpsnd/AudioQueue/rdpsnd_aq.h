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

#ifndef _RDP_SND_AQ_H_
#define _RDP_SND_AQ_H_


#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include <freerdp/freerdp.h>
#include <freerdp/utils/memory.h>
#include <freerdp/utils/dsp.h>
#include <freerdp/utils/svc_plugin.h>

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

#include "rdpsnd_main.h"


typedef struct rdpsnd_aq_buffer {
    AudioQueueBufferRef buf;
    boolean enqueued;
} rdpsndAudioQueueBuffer;

typedef struct rdpsnd_qa_block {
    void *ptr;
    sint32 size;
} rdpsndAudioQueueBlock;

typedef struct rdpsnd_aq_plugin {
    rdpsndDevicePlugin mDevice;

    char *mDeviceName;
    sint32 mFormat;
    sint32 mBlockSize;
    sint32 mLatency;
    ADPCM mAdpcm;

    boolean mIsRunning;
    AudioStreamBasicDescription mDataFormat;
    AudioQueueRef mQueue;
    rdpsndAudioQueueBuffer *mBuffers;
    sint32 mNumBuffers;
    pthread_mutex_t mBufLock;
    uint32 mBufferByteSize;
    sint64 mCurrentPacket;
    uint32 mNumPacketsToRead;
    AudioStreamPacketDescription *mPacketDescs;

    // The data blocks pending to be enqueued
    CFMutableArrayRef mPendingBlocks;
} rdpsndAudioQueuePlugin;


#endif
