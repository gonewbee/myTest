//
// Created by zsy on 16-4-6.
//

#ifndef OPENGLES_GLES2_TEXTURE_H
#define OPENGLES_GLES2_TEXTURE_H

#include "gles2_render.h"

void setup_texture_VBOs(UserContext *context);
int texture_create(UserContext *context);
int texture_draw(UserContext *context);

#endif //OPENGLES_GLES2_TEXTURE_H
