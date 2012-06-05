/**
 * FreeRDP: A Remote Desktop Protocol Client
 * Android Client
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

#include "android_graphics.h"
#include "androidfreerdp.h"


static freerdp_sem g_sem;
static int g_thread_count = 0;

static struct FreeRDPHostAPI *g_host_api = NULL;

struct thread_data
{
	freerdp* instance;
};


void android_context_new(freerdp* instance, rdpContext* context)
{
	context->channels = freerdp_channels_new();
}

void android_context_free(freerdp* instance, rdpContext* context)
{

}

void android_begin_paint(rdpContext* context)
{
	rdpGdi* gdi = context->gdi;
	gdi->primary->hdc->hwnd->invalid->null = 1;
}

void android_end_paint(rdpContext* context)
{
    rdpGdi* gdi;
    androidInfo* androidi;

    gdi = context->gdi;
    androidi = ((androidContext *) context)->androidi;

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

boolean android_get_fds(freerdp* instance, void** rfds, int* rcount, void** wfds, int* wcount)
{
#if 0
    androidInfo* androidi;

    androidi = ((androidContext *) instance->context)->androidi;

    rfds[*rcount] = (void *)(long)(androidi->read_fds);
    (*rcount)++;
#endif

    return true;
}

boolean android_check_fds(freerdp* instance, fd_set* set)
{
#if 0
	androidInfo* androidi;

	androidi = ((androidContext *) instance->context)->androidi;

	if (!FD_ISSET(androidi->read_fds, set))
		return true;

	if (read(androidi->read_fds, &(dfi->event), sizeof(dfi->event)) > 0)
		android_event_process(instance, &(dfi->event));
#endif

	return true;
}

boolean android_pre_connect(freerdp* instance)
{
	androidInfo* androidi;
	boolean bitmap_cache;
	androidContext* context;
	rdpSettings* settings;

	androidi = (androidInfo *) xzalloc(sizeof(androidInfo));
	context = ((androidContext *) instance->context);
	context->androidi = androidi;

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

	androidi->clrconv = xnew(CLRCONV);
	androidi->clrconv->alpha = 1;
	androidi->clrconv->invert = 0;
	androidi->clrconv->rgb555 = 0;
	androidi->clrconv->palette = xnew(rdpPalette);

	freerdp_channels_pre_connect(instance->context->channels, instance);

	return true;
}

boolean android_post_connect(freerdp* instance)
{
	rdpGdi* gdi;
	androidInfo* androidi;
	androidContext* context;

	context = ((androidContext*) instance->context);
	androidi = context->androidi;

	gdi_init(instance, CLRCONV_ALPHA | CLRCONV_INVERT | CLRBUF_16BPP | CLRBUF_32BPP, NULL);
	gdi = instance->context->gdi;

	instance->update->BeginPaint = android_begin_paint;
	instance->update->EndPaint = android_end_paint;

	pointer_cache_register_callbacks(instance->update);
	android_register_graphics(instance->context->graphics);

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

static int android_process_plugin_args(rdpSettings* settings, const char* name,
                                   RDP_PLUGIN_DATA* plugin_data, void* user_data)
{
	rdpChannels* channels = (rdpChannels*) user_data;

	printf("loading plugin %s\n", name);
	freerdp_channels_load_plugin(channels, settings, name, plugin_data);

	return 1;
}

boolean android_verify_certificate(freerdp* instance, char* subject, char* issuer, char* fingerprint)
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
android_receive_channel_data(freerdp* instance, int channelId, uint8* data, int size, int flags, int total_size)
{
	return freerdp_channels_data(instance, channelId, data, size, flags, total_size);
}

static void
android_process_cb_monitor_ready_event(rdpChannels* channels, freerdp* instance)
{
	RDP_EVENT* event;
	RDP_CB_FORMAT_LIST_EVENT* format_list_event;

	event = freerdp_event_new(RDP_EVENT_CLASS_CLIPRDR, RDP_EVENT_TYPE_CB_FORMAT_LIST, NULL, NULL);

	format_list_event = (RDP_CB_FORMAT_LIST_EVENT*)event;
	format_list_event->num_formats = 0;

	freerdp_channels_send_event(channels, event);
}

static void
android_process_channel_event(rdpChannels* channels, freerdp* instance)
{
    RDP_EVENT* event;

    event = freerdp_channels_pop_event(channels);

    if (event) {
        switch (event->event_type) {
        case RDP_EVENT_TYPE_CB_MONITOR_READY:
            android_process_cb_monitor_ready_event(channels, instance);
            break;
        default:
            printf("android_process_channel_event: unknown event type %d\n", event->event_type);
            break;
        }

        freerdp_event_free(event);
    }
}

static void android_free(androidInfo* androidi)
{
    xfree(androidi);
}

int androidfreerdp_run(freerdp* instance)
{
	int i;
	int fds;
	int max_fds;
	int rcount;
	int wcount;
	void* rfds[32];
	void* wfds[32];
	fd_set rfds_set;
	fd_set wfds_set;
	androidInfo* androidi;
	androidContext* context;
	rdpChannels* channels;

	memset(rfds, 0, sizeof(rfds));
	memset(wfds, 0, sizeof(wfds));

	if (!freerdp_connect(instance))
		return 0;

	context = (androidContext*) instance->context;

	androidi = context->androidi;
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
		if (android_get_fds(instance, rfds, &rcount, wfds, &wcount) != true)
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
				printf("androidfreerdp_run: select failed\n");
				break;
			}
		}

		if (freerdp_check_fds(instance) != true)
		{
			printf("Failed to check FreeRDP file descriptor\n");
			break;
		}
		if (android_check_fds(instance, &rfds_set) != true)
		{
			printf("Failed to check androidfreerdp file descriptor\n");
			break;
		}
		if (freerdp_channels_check_fds(channels, instance) != true)
		{
			printf("Failed to check channel manager file descriptor\n");
			break;
		}
		android_process_channel_event(channels, instance);
	}

	freerdp_channels_close(channels, instance);
	freerdp_channels_free(channels);
	android_free(androidi);
	gdi_free(instance);
	freerdp_disconnect(instance);
	freerdp_free(instance);

	return 0;
}

void* thread_func(void* param)
{
	struct thread_data* data;
	data = (struct thread_data*) param;

	androidfreerdp_run(data->instance);

	xfree(data);

	pthread_detach(pthread_self());

	g_thread_count--;

        if (g_thread_count < 1)
                freerdp_sem_signal(g_sem);

	return NULL;
}

static freerdp*
androidfreerdp_new(int argc, char* argv[])
{
	pthread_t thread;
	freerdp* instance;
	androidContext* context;
	rdpChannels* channels;
	struct thread_data* data;

        g_thread_count = 0;

	setlocale(LC_ALL, "");

	freerdp_channels_global_init();

	g_sem = freerdp_sem_new(1);

	instance = freerdp_new();
	instance->PreConnect = android_pre_connect;
	instance->PostConnect = android_post_connect;
	instance->VerifyCertificate = android_verify_certificate;
	instance->ReceiveChannelData = android_receive_channel_data;

	instance->context_size = sizeof(androidContext);
	instance->ContextNew = android_context_new;
	instance->ContextFree = android_context_free;
	freerdp_context_new(instance);

	context = (androidContext*) instance->context;
	channels = instance->context->channels;

        //TODO android specific initialization

	freerdp_parse_args(instance->settings, argc, argv, android_process_plugin_args, channels, NULL, NULL);

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

    argv = (char **)xmalloc(sizeof(char *) * 13);
    bzero(argv, 13 * sizeof(char *));
    bzero(geometry, 128);

    sprintf(geometry, "%dx%d", width, height);

    argv[0] = strdup("androidfreerdp");
    argv[1] = strdup("-u");
    argv[2] = strdup(username);
    argv[3] = strdup("-p");
    argv[4] = strdup(pass);
    argv[5] = strdup("-a");
    argv[6] = strdup("32");
    argv[7] = strdup("-g");
    argv[8] = strdup(geometry);
    argv[9] = strdup("-f");
    if (domain) {
        argv[10] = strdup("-d");
        argv[11] = strdup(domain);
        argv[12] = strdup(host);
        argc = 13;
    } else {
        argv[10] = strdup(host);
        argc = 11;
    }

    printf("%s: connect to %s %s:%s\n", __func__, host, username, pass);
    ret = androidfreerdp_new(argc, argv);

    for (i = 0; i < 13; i++) {
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

void android_main()
{
    char *host="192.168.0.179";
    char *username="orchard";
    char *pass="1234";
    char *domain="";
    int width=800;
    int height=600;
    freerdp_ext_connect(*host, *username,*pass, *domain, width, height);
}
