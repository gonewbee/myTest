//
// Created by zsy on 2/24/16.
//

#include <unistd.h>
#include "agent_client.h"
#include "agentrdp.h"
#include "freerdp/client/channels.h"
#include "freerdp/client/cmdline.h"
#include "freerdp/cache/cache.h"
#include "freerdp/gdi/gdi.h"
#include "encoder.h"

static BOOL agent_begin_paint(rdpContext* context) {
    rdpGdi* gdi = context->gdi;
    gdi->primary->hdc->hwnd->invalid->null = 1;
    gdi->primary->hdc->hwnd->ninvalid = 0;
    return TRUE;
}

static BOOL agent_end_paint(rdpContext* context) {
//    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}

static BOOL agent_desktop_resize(rdpContext* context) {
    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}

static BOOL agent_pre_connect(freerdp* instance) {
    rdpSettings* settings = instance->settings;
    BOOL bitmap_cache = settings->BitmapCacheEnabled;
    settings->OrderSupport[NEG_DSTBLT_INDEX] = TRUE;
    settings->OrderSupport[NEG_PATBLT_INDEX] = TRUE;
    settings->OrderSupport[NEG_SCRBLT_INDEX] = TRUE;
    settings->OrderSupport[NEG_OPAQUE_RECT_INDEX] = TRUE;
    settings->OrderSupport[NEG_DRAWNINEGRID_INDEX] = FALSE;
    settings->OrderSupport[NEG_MULTIDSTBLT_INDEX] = FALSE;
    settings->OrderSupport[NEG_MULTIPATBLT_INDEX] = FALSE;
    settings->OrderSupport[NEG_MULTISCRBLT_INDEX] = FALSE;
    settings->OrderSupport[NEG_MULTIOPAQUERECT_INDEX] = TRUE;
    settings->OrderSupport[NEG_MULTI_DRAWNINEGRID_INDEX] = FALSE;
    settings->OrderSupport[NEG_LINETO_INDEX] = TRUE;
    settings->OrderSupport[NEG_POLYLINE_INDEX] = TRUE;
    settings->OrderSupport[NEG_MEMBLT_INDEX] = bitmap_cache;
    settings->OrderSupport[NEG_MEM3BLT_INDEX] = TRUE;
    settings->OrderSupport[NEG_MEMBLT_V2_INDEX] = bitmap_cache;
    settings->OrderSupport[NEG_MEM3BLT_V2_INDEX] = FALSE;
    settings->OrderSupport[NEG_SAVEBITMAP_INDEX] = FALSE;
    settings->OrderSupport[NEG_GLYPH_INDEX_INDEX] = TRUE;
    settings->OrderSupport[NEG_FAST_INDEX_INDEX] = TRUE;
    settings->OrderSupport[NEG_FAST_GLYPH_INDEX] = TRUE;
    settings->OrderSupport[NEG_POLYGON_SC_INDEX] = FALSE;
    settings->OrderSupport[NEG_POLYGON_CB_INDEX] = FALSE;
    settings->OrderSupport[NEG_ELLIPSE_SC_INDEX] = FALSE;
    settings->OrderSupport[NEG_ELLIPSE_CB_INDEX] = FALSE;

    settings->FrameAcknowledge = 10;

//    PubSub_SubscribeChannelConnected(instance->context->pubSub,
//                                     (pChannelConnectedEventHandler) agent_OnChannelConnectedEventHandler);
//
//    PubSub_SubscribeChannelDisconnected(instance->context->pubSub,
//                                        (pChannelDisconnectedEventHandler) agent_OnChannelDisconnectedEventHandler);

    freerdp_register_addin_provider(freerdp_channels_load_static_addin_entry, 0);
    freerdp_client_load_addins(instance->context->channels, instance->settings);

    freerdp_channels_pre_connect(instance->context->channels, instance);

    return TRUE;
}

static BOOL agent_post_connect(freerdp* instance) {
    UINT32 gdi_flags;
    rdpSettings *settings = instance->settings;

    if (!(instance->context->cache = cache_new(settings)))
        return FALSE;

    if (instance->settings->ColorDepth > 16)
        gdi_flags = CLRBUF_32BPP | CLRCONV_ALPHA | CLRCONV_INVERT;
    else
        gdi_flags = CLRBUF_16BPP;

    if (!gdi_init(instance, gdi_flags, NULL))
        return FALSE;

    instance->update->BeginPaint = agent_begin_paint;
    instance->update->EndPaint = agent_end_paint;
    instance->update->DesktopResize = agent_desktop_resize;

    if (freerdp_channels_post_connect(instance->context->channels, instance) < 0)
        return FALSE;

    return TRUE;
}

static void agent_post_disconnect(freerdp* instance) {
    gdi_free(instance);
    cache_free(instance->context->cache);
}

static BOOL agent_authenticate(freerdp* instance, char** username, char** password, char** domain) {
    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}

static BOOL agent_verify_certificate(freerdp* instance, char* subject, char* issuer, char* fingerprint) {
    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}

static BOOL agent_verify_changed_certificate(freerdp* instance, char* subject, char* issuer,
                                               char* new_fingerprint, char* old_subject, char* old_issuer, char* old_fingerprint) {
    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}


static BOOL agent_client_global_init() {
    fprintf(stdout, "%s enter!\n", __func__);
    return TRUE;
}

static void agent_client_global_uninit() {
    fprintf(stdout, "%s enter!\n", __func__);
}

static BOOL agent_client_new(freerdp* instance, rdpContext* context) {
    if (!(context->channels = freerdp_channels_new())) {
        return FALSE;
    }
    instance->PreConnect = agent_pre_connect;
    instance->PostConnect = agent_post_connect;
    instance->PostDisconnect = agent_post_disconnect;
    instance->Authenticate = agent_authenticate;
    instance->VerifyCertificate = agent_verify_certificate;
    instance->VerifyChangedCertificate = agent_verify_changed_certificate;
    return TRUE;
}

static void agent_client_free(freerdp* instance, rdpContext* context) {
    if (context && context->channels) {
        freerdp_channels_close(context->channels, instance);
        freerdp_channels_free(context->channels);
        context->channels = NULL;
    }
}

static int agent_client_start(rdpContext* context) {
    BOOL status;
    DWORD nCount;
    DWORD waitStatus;
    HANDLE handles[64];
    agentContext* xfc;
    freerdp* instance;

    instance = context->instance;
    context = instance->context;

    fprintf(stdout, "%s enter!\n", __func__);

    status = freerdp_connect(instance);
    if (!status) {
        fprintf(stderr, "freerdp_connect error!!\n");
        return 1;
    }

    xfc = (agentContext*) instance->context;
    while (!xfc->disconnect && !freerdp_shall_disconnect(instance)) {
        nCount = 0;
        DWORD tmp = freerdp_get_event_handles(context, &handles[nCount], 64 - nCount);
        if (tmp == 0)
        {
            fprintf(stderr, "freerdp_get_event_handles failed\n");
            break;
        }
        nCount += tmp;

        waitStatus = WaitForMultipleObjects(nCount, handles, FALSE, 100);
        if (waitStatus!=0 && waitStatus!=258) {
            fprintf(stdout, "waitStatus:%d\n", waitStatus);
        }

        if (!freerdp_check_event_handles(context))
        {
            fprintf(stderr, "Failed to check FreeRDP file descriptor\n");
            break;
        }
    }
    fprintf(stdout, "web_client_func==========end\n");
    freerdp_disconnect(instance);

    return 0;
}

static int agent_client_stop(rdpContext* context) {
    fprintf(stdout, "%s enter!\n", __func__);
    return 0;
}

int RdpClientEntry(RDP_CLIENT_ENTRY_POINTS* pEntryPoints)
{
    pEntryPoints->Version = 1;
    pEntryPoints->Size = sizeof(RDP_CLIENT_ENTRY_POINTS_V1);
    pEntryPoints->GlobalInit = agent_client_global_init;
    pEntryPoints->GlobalUninit = agent_client_global_uninit;
    pEntryPoints->ContextSize = sizeof(agentContext);
    pEntryPoints->ClientNew = agent_client_new;
    pEntryPoints->ClientFree = agent_client_free;
    pEntryPoints->ClientStart = agent_client_start;
    pEntryPoints->ClientStop = agent_client_stop;
    return 0;
}

/**
 * @breif 将gdi中的像素数据保存到文件
 */
void save2file(rdpGdi *gdi) {
    static int saveNum = 0;
    char name[64] = {0};
    int size = gdi->width*gdi->height*gdi->bytesPerPixel;
    snprintf(name, 63, "%u_%u_%u_%u.rgb565", gdi->width, gdi->height, gdi->bytesPerPixel, saveNum);
    fprintf(stdout, "filename:%s size:%d\n", name, size);
    saveNum += 1;
    FILE *f = fopen(name, "w");
    if (!f) {
        perror("fopen");
        return;
    }
    fwrite(gdi->primary_buffer, 1, size, f);
    fclose(f);
}

void* agent_capture_gdi_thread(void* param) {
    freerdp* instance;
    instance = (freerdp*) param;
    jsmpeg_frame_t *frame = (jsmpeg_frame_t *)malloc(APP_FRAME_BUFFER_SIZE);
    size_t encoded_size = APP_FRAME_BUFFER_SIZE - sizeof(jsmpeg_frame_t);
    sleep(2);
    rdpGdi *gdi = instance->context->gdi;
    encoder_t *encoder = encoder_create(
            gdi->width, gdi->height, // in size
            1024, 768, // out size
            2000000
    );
    FILE *f = fopen("test.mpeg", "w");
    while (1) {
        fprintf(stdout, "%d %d %d\n", gdi->width, gdi->height, gdi->bytesPerPixel);
//        save2file(gdi);
        usleep(100*1000);
        encoded_size = APP_FRAME_BUFFER_SIZE - sizeof(jsmpeg_frame_t);
        encoder_encode(encoder, gdi->primary_buffer, frame->data, &encoded_size);
        fprintf(stdout, "encoded_size:%d\n", (int)encoded_size);
        fwrite(frame->data, 1, encoded_size, f);
    }
    fclose(f);
    encoder_destroy(encoder);
}

int agent_start_capture(rdpContext* context) {
    agentContext* xfc = (agentContext*) context;
    if (!(xfc->capture_thread = CreateThread(NULL, 0,
                                     (LPTHREAD_START_ROUTINE) agent_capture_gdi_thread,
                                     context->instance, 0, NULL))) {
        fprintf(stderr, "failed to create client thread");
        return -1;
    }
    return 0;
}
