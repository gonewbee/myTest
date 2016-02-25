//
// Created by zsy on 2/25/16.
//

#ifndef AGENTRDP_ENCODER_H
#define AGENTRDP_ENCODER_H

#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>

#define APP_FRAME_BUFFER_SIZE (1024*1024)

typedef struct {
	AVCodec *codec;
	AVCodecContext *context;
	AVFrame *frame;
	void *frame_buffer;

	int in_width, in_height;
	int out_width, out_height;

	AVPacket packet;
    struct SwsContext *sws;
} encoder_t;

typedef enum {
    jsmpeg_frame_type_video = 0xFA010000,
    jsmpeg_frame_type_audio = 0xFB010000
} jsmpeg_trame_type_t;

typedef struct {
    jsmpeg_trame_type_t type;
    int size;
    char data[0];
} jsmpeg_frame_t;

encoder_t *encoder_create(int in_width, int in_height, int out_width, int out_height, int bitrate);
void encoder_destroy(encoder_t *self);
void encoder_encode(encoder_t *self, void *rgb_pixels, void *encoded_data, size_t *encoded_size);

#endif //AGENTRDP_ENCODER_H
