/**
 * FreeRDP: A Remote Desktop Protocol Client
 * iOS Client
 *
 * Copyright 2011 Nevo <snowgeekx@gmail.com>
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

#include <errno.h>
#include <pthread.h>
#include <locale.h>
#include <freerdp/locale/keyboard.h>
#include <freerdp/utils/args.h>
#include <freerdp/utils/memory.h>
#include <freerdp/utils/semaphore.h>
#include <freerdp/utils/event.h>
#include <freerdp/constants.h>
#include <freerdp/plugins/cliprdr.h>

#include "ios_graphics.h"
#include "iosfreerdp.h"


static freerdp_sem g_sem;
static int g_thread_count = 0;

static struct FreeRDPHostAPI *g_host_api = NULL;

struct thread_data
{
	freerdp* instance;
};


void ios_context_new(freerdp* instance, rdpContext* context)
{
	context->channels = freerdp_channels_new();
}

void ios_context_free(freerdp* instance, rdpContext* context)
{

}

void ios_begin_paint(rdpContext* context)
{
	rdpGdi* gdi = context->gdi;
	gdi->primary->hdc->hwnd->invalid->null = 1;
}

void ios_end_paint(rdpContext* context)
{
    rdpGdi* gdi;
    iosInfo* iosi;

    gdi = context->gdi;
    iosi = ((iosContext *) context)->iosi;

    if (gdi->primary->hdc->hwnd->invalid->null)
        return;

    g_host_api->updateScreen(g_host_api->hostContext,
                             gdi->primary->hdc->hwnd->invalid->x,
                             gdi->primary->hdc->hwnd->invalid->y,
                             gdi->primary->hdc->hwnd->invalid->x +
                             gdi->primary->hdc->hwnd->invalid->w,
                             gdi->primary->hdc->hwnd->invalid->y +
                             gdi->primary->hdc->hwnd->invalid->h);
}

boolean ios_get_fds(freerdp* instance, void** rfds, int* rcount, void** wfds, int* wcount)
{
#if 0
    iosInfo* iosi;

    iosi = ((iosContext *) instance->context)->iosi;

    rfds[*rcount] = (void *)(long)(iosi->read_fds);
    (*rcount)++;
#endif

    return true;
}

boolean ios_check_fds(freerdp* instance, fd_set* set)
{
#if 0
	iosInfo* iosi;

	iosi = ((iosContext *) instance->context)->iosi;

	if (!FD_ISSET(iosi->read_fds, set))
		return true;

	if (read(iosi->read_fds, &(dfi->event), sizeof(dfi->event)) > 0)
		ios_event_process(instance, &(dfi->event));
#endif

	return true;
}

boolean ios_pre_connect(freerdp* instance)
{
	iosInfo* iosi;
	boolean bitmap_cache;
	iosContext* context;
	rdpSettings* settings;

	iosi = (iosInfo *) xzalloc(sizeof(iosInfo));
	context = ((iosContext *) instance->context);
	context->iosi = iosi;

	settings = instance->settings;
	bitmap_cache = settings->bitmap_cache;

	settings->order_support[NEG_DSTBLT_INDEX] = true;
	settings->order_support[NEG_PATBLT_INDEX] = true;
	settings->order_support[NEG_SCRBLT_INDEX] = true;
	settings->order_support[NEG_OPAQUE_RECT_INDEX] = true;
	settings->order_support[NEG_DRAWNINEGRID_INDEX] = false;
	settings->order_support[NEG_MULTIDSTBLT_INDEX] = false;
	settings->order_support[NEG_MULTIPATBLT_INDEX] = false;
	settings->order_support[NEG_MULTISCRBLT_INDEX] = false;
	settings->order_support[NEG_MULTIOPAQUERECT_INDEX] = true;
	settings->order_support[NEG_MULTI_DRAWNINEGRID_INDEX] = false;
	settings->order_support[NEG_LINETO_INDEX] = true;
	settings->order_support[NEG_POLYLINE_INDEX] = true;
	settings->order_support[NEG_MEMBLT_INDEX] = bitmap_cache;
	settings->order_support[NEG_MEM3BLT_INDEX] = false;
	settings->order_support[NEG_MEMBLT_V2_INDEX] = bitmap_cache;
	settings->order_support[NEG_MEM3BLT_V2_INDEX] = false;
	settings->order_support[NEG_SAVEBITMAP_INDEX] = false;
	settings->order_support[NEG_GLYPH_INDEX_INDEX] = false;
	settings->order_support[NEG_FAST_INDEX_INDEX] = false;
	settings->order_support[NEG_FAST_GLYPH_INDEX] = false;
	settings->order_support[NEG_POLYGON_SC_INDEX] = false;
	settings->order_support[NEG_POLYGON_CB_INDEX] = false;
	settings->order_support[NEG_ELLIPSE_SC_INDEX] = false;
	settings->order_support[NEG_ELLIPSE_CB_INDEX] = false;

	iosi->clrconv = xnew(CLRCONV);
	iosi->clrconv->alpha = 1;
	iosi->clrconv->invert = 0;
	iosi->clrconv->rgb555 = 0;
	iosi->clrconv->palette = xnew(rdpPalette);

	freerdp_channels_pre_connect(instance->context->channels, instance);

	return true;
}

boolean ios_post_connect(freerdp* instance)
{
	rdpGdi* gdi;
	iosInfo* iosi;
	iosContext* context;

	context = ((iosContext*) instance->context);
	iosi = context->iosi;

	gdi_init(instance, CLRCONV_ALPHA | CLRCONV_INVERT | CLRBUF_16BPP | CLRBUF_32BPP, NULL);
	gdi = instance->context->gdi;

	instance->update->BeginPaint = ios_begin_paint;
	instance->update->EndPaint = ios_end_paint;

	pointer_cache_register_callbacks(instance->update);
	ios_register_graphics(instance->context->graphics);

	freerdp_channels_post_connect(instance->context->channels, instance);

        g_host_api->setDisplayMode(g_host_api->hostContext,
                                   instance->settings->color_depth,
                                   instance->settings->color_depth,
                                   instance->settings->color_depth *
                                   instance->settings->width / 8,
                                   instance->settings->width,
                                   instance->settings->height,
                                   gdi->primary_buffer);

	return true;
}

static int ios_process_plugin_args(rdpSettings* settings, const char* name,
                                   RDP_PLUGIN_DATA* plugin_data, void* user_data)
{
    static char plugin_args[4][256] = { { 0 } };

    int i;
    rdpChannels* channels = (rdpChannels*) user_data;

    printf("loading plugin %s\n", name);

    /*
     * FIXME Original freerdp just passes argv[index] data pointer
     *       to plugin runtime as plugin data, it's managed by
     *       C runtime nevertheless. So just explicitly make a static
     *       copy here to avoid crashing the plugin loader.
     */
    for (i = 0; i < 4; i++) {
        if (plugin_data->data[i]) {
            char *data = plugin_data->data[i];
            strcpy(plugin_args[i], data);
            plugin_data->data[i] = plugin_args[i];
        }
    }
    freerdp_channels_load_plugin(channels, settings, name, plugin_data);

    return 1;
}

boolean ios_verify_certificate(freerdp* instance, char* subject, char* issuer, char* fingerprint)
{
	printf("Certificate details:\n");
	printf("\tSubject: %s\n", subject);
	printf("\tIssuer: %s\n", issuer);
	printf("\tThumbprint: %s\n", fingerprint);
	printf("The above X.509 certificate could not be verified, possibly because you do not have "
		"the CA certificate in your certificate store, or the certificate has expired. "
		"Please look at the documentation on how to create local certificate store for a private CA.\n");

        // TODO X509 Certificate verification
        return true;
#if 0
	char answer;
	while (1)
	{
		printf("Do you trust the above certificate? (Y/N) ");
		answer = fgetc(stdin);

		if (answer == 'y' || answer == 'Y')
		{
			return true;
		}
		else if (answer == 'n' || answer == 'N')
		{
			break;
		}
	}

	return false;
#endif
}

static int
ios_receive_channel_data(freerdp* instance, int channelId, uint8* data, int size, int flags, int total_size)
{
	return freerdp_channels_data(instance, channelId, data, size, flags, total_size);
}

static void
ios_process_cb_monitor_ready_event(rdpChannels* channels, freerdp* instance)
{
	RDP_EVENT* event;
	RDP_CB_FORMAT_LIST_EVENT* format_list_event;

	event = freerdp_event_new(RDP_EVENT_CLASS_CLIPRDR, RDP_EVENT_TYPE_CB_FORMAT_LIST, NULL, NULL);

	format_list_event = (RDP_CB_FORMAT_LIST_EVENT*)event;
	format_list_event->num_formats = 0;

	freerdp_channels_send_event(channels, event);
}

static void
ios_process_channel_event(rdpChannels* channels, freerdp* instance)
{
    RDP_EVENT* event;

    event = freerdp_channels_pop_event(channels);

    if (event) {
        switch (event->event_type) {
        case RDP_EVENT_TYPE_CB_MONITOR_READY:
            ios_process_cb_monitor_ready_event(channels, instance);
            break;
        default:
            printf("ios_process_channel_event: unknown event type %d\n", event->event_type);
            break;
        }

        freerdp_event_free(event);
    }
}

static void ios_free(iosInfo* iosi)
{
    xfree(iosi);
}

int iosfreerdp_run(freerdp* instance)
{
	int i;
	int fds;
	int max_fds;
	int rcount;
	int wcount;
        int ret = 0;
	void* rfds[32];
	void* wfds[32];
	fd_set rfds_set;
	fd_set wfds_set;
	iosInfo* iosi;
	iosContext* context;
	rdpChannels* channels;

	memset(rfds, 0, sizeof(rfds));
	memset(wfds, 0, sizeof(wfds));

	if (!freerdp_connect(instance))
		return 0;

	context = (iosContext*) instance->context;

	iosi = context->iosi;
	channels = instance->context->channels;

	while (g_thread_count) {
		rcount = 0;
		wcount = 0;

		if (freerdp_get_fds(instance, rfds, &rcount, wfds, &wcount) != true)
		{
			printf("Failed to get FreeRDP file descriptor\n");
			break;
		}
		if (freerdp_channels_get_fds(channels, instance, rfds, &rcount, wfds, &wcount) != true)
		{
			printf("Failed to get channel manager file descriptor\n");
			break;
		}
		if (ios_get_fds(instance, rfds, &rcount, wfds, &wcount) != true)
		{
			printf("Failed to get dfreerdp file descriptor\n");
			break;
		}

		max_fds = 0;
		FD_ZERO(&rfds_set);
		FD_ZERO(&wfds_set);

		for (i = 0; i < rcount; i++)
		{
			fds = (int)(long)(rfds[i]);

			if (fds > max_fds)
				max_fds = fds;

			FD_SET(fds, &rfds_set);
		}

		if (max_fds == 0)
			break;

		if (select(max_fds + 1, &rfds_set, &wfds_set, NULL, NULL) == -1)
		{
			/* these are not really errors */
			if (!((errno == EAGAIN) ||
				(errno == EWOULDBLOCK) ||
				(errno == EINPROGRESS) ||
				(errno == EINTR))) /* signal occurred */
			{
				printf("iosfreerdp_run: select failed\n");
				break;
			}
		}

		if (freerdp_check_fds(instance) != true)
		{
			printf("Failed to check FreeRDP file descriptor\n");
			break;
		}
		if (ios_check_fds(instance, &rfds_set) != true)
		{
			printf("Failed to check iosfreerdp file descriptor\n");
			break;
		}
		if (freerdp_channels_check_fds(channels, instance) != true)
		{
			printf("Failed to check channel manager file descriptor\n");
			break;
		}
		ios_process_channel_event(channels, instance);
	}

        //TODO Add client local error info
        ret = freerdp_error_info(instance);

	freerdp_channels_close(channels, instance);
	freerdp_channels_free(channels);
	ios_free(iosi);
	gdi_free(instance);
	freerdp_disconnect(instance);
	freerdp_free(instance);

	return ret;
}

void* thread_func(void* param)
{
    int disconnect_reason = 0;
    struct thread_data* data;
    data = (struct thread_data*) param;

    disconnect_reason = iosfreerdp_run(data->instance);
    g_host_api->disconnectWithReason(g_host_api->hostContext,
                                     disconnect_reason);

    xfree(data);

    pthread_detach(pthread_self());

    g_thread_count--;

    if (g_thread_count < 1) {
        freerdp_sem_signal(g_sem);
    }

    return NULL;
}

static freerdp*
iosfreerdp_new(int argc, char* argv[])
{
	pthread_t thread;
	freerdp* instance;
	iosContext* context;
	rdpChannels* channels;
	struct thread_data* data;

        g_thread_count = 0;

	setlocale(LC_ALL, "");

	freerdp_channels_global_init();

	g_sem = freerdp_sem_new(1);

	instance = freerdp_new();
	instance->PreConnect = ios_pre_connect;
	instance->PostConnect = ios_post_connect;
	instance->VerifyCertificate = ios_verify_certificate;
	instance->ReceiveChannelData = ios_receive_channel_data;

	instance->context_size = sizeof(iosContext);
	instance->ContextNew = ios_context_new;
	instance->ContextFree = ios_context_free;
	freerdp_context_new(instance);

	context = (iosContext*) instance->context;
	channels = instance->context->channels;

        //TODO iOS specific initialization

	freerdp_parse_args(instance->settings, argc, argv, ios_process_plugin_args, channels, NULL, NULL);

	data = (struct thread_data*) xzalloc(sizeof(struct thread_data));
	data->instance = instance;

	g_thread_count++;
	pthread_create(&thread, 0, thread_func, data);

#if 0
	while (g_thread_count > 0)
	{
                freerdp_sem_wait(g_sem);
	}

	freerdp_channels_global_uninit();
#endif

	return instance;
}

void
freerdp_ext_disconnect(freerdp *session) // NOT USED
{
    printf("%s: disconnecting FreeRDP\n", __func__);
    if (g_thread_count > 0) {
        // Signal rdp thread to quit
	g_thread_count = 0;
        freerdp_sem_wait(g_sem);
    }

    freerdp_channels_global_uninit();
    xfree(g_sem);
    printf("%s: FreeRDP is successfully disconnected\n", __func__);
}

void
freerdp_ext_register_host_api(struct FreeRDPHostAPI *hostApi)
{
    free(g_host_api);
    if (hostApi) {
        g_host_api = malloc(sizeof(struct FreeRDPHostAPI));
        memcpy(g_host_api, hostApi, sizeof(struct FreeRDPHostAPI));
    } else {
        g_host_api = NULL;
    }
}

freerdp*
freerdp_ext_connect(const char *host, const char *username,
                    const char *pass, const char *domain,
                    int width, int height)
{
    char geometry[128];
    char **argv;
    int i;
    int argc = 0;
    freerdp *ret;

    argv = (char **)xmalloc(sizeof(char *) * 18);
    bzero(argv, 18 * sizeof(char *));
    bzero(geometry, 128);

    sprintf(geometry, "%dx%d", width, height);

    argv[0] = strdup("iosfreerdp");
    argv[1] = strdup("-u");
    argv[2] = strdup(username);
    argv[3] = strdup("-p");
    argv[4] = strdup(pass);
    argv[5] = strdup("-a");
    argv[6] = strdup("32");
    argv[7] = strdup("-g");
    argv[8] = strdup(geometry);
    argv[9] = strdup("-f");
    argv[10] = strdup("--plugin");
    argv[11] = strdup("rdpsnd");
    argv[12] = strdup("--data");
    argv[13] = strdup("aq");
    argv[14] = strdup("--");
    if (domain) {
        argv[15] = strdup("-d");
        argv[16] = strdup(domain);
        argv[17] = strdup(host);
        argc = 18;
    } else {
        argv[15] = strdup(host);
        argc = 16;
    }

    printf("%s: connect to %s %s:%s\n", __func__, host, username, pass);
    ret = iosfreerdp_new(argc, argv);

    for (i = 0; i < 18; i++) {
        free(argv[i]);
    }
    free(argv);

    return ret;
}

void
freerdp_ext_send_mouse_event(freerdp *session,
                             int cursorx, int cursory,
                             int button0, int button1,
                             int button2, int button3,
                             int button4, uint8 down)
{
    boolean wheel = false;
    uint16 device_flags = 0;

    if (down) {
        device_flags = PTR_FLAGS_DOWN;
    }

    if (button0) {
        device_flags |= PTR_FLAGS_BUTTON1;
    }
    if (button1) {
        device_flags |= PTR_FLAGS_BUTTON2;
    }
    if (button2) {
        device_flags |= PTR_FLAGS_BUTTON3;
    }

    /*
     * The UI should scroll when the accumulated delta is plus or minus
     * 120 (0x0078 and 0x0088).
     * See: http://msdn.microsoft.com/en-us/library/system.windows.forms.control.mousewheel(v=vs.71).aspx
     */
    if (button3) {
        // Away from you
        wheel = true;
        device_flags |= PTR_FLAGS_WHEEL | 0x0078;
    }
    if (button4) {
        // Toward you
        wheel = true;
        device_flags |= PTR_FLAGS_WHEEL | PTR_FLAGS_WHEEL_NEGATIVE | 0x0088;
    }

    if (device_flags && session) {
        session->input->MouseEvent(session->input, device_flags,
                                   wheel ? 0 : cursorx,
                                   wheel ? 0 : cursory);
    }
}

void
freerdp_ext_send_key_event(freerdp *session,
                           boolean unicode, boolean up,
                           int vk)
{
    uint16 flags = 0;
    uint16 uc;
    uint8 scancode;

    if (unicode) {
        uc = (uint16)vk;
    } else {
        scancode = freerdp_keyboard_get_rdp_scancode_from_virtual_key_code(vk);
    }

    flags |= up ? KBD_FLAGS_RELEASE : KBD_FLAGS_DOWN;

    if (session) {
#if 0
        printf("send_key_event: up %d keycode %d extended %d\n", up,
               scancode, extended);
#endif
        if (unicode) {
            session->input->UnicodeKeyboardEvent(session->input, flags, uc);
        } else {
            session->input->KeyboardEvent(session->input, flags, scancode);
        }
    }
}
