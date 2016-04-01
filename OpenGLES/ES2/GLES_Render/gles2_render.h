//
// Created by zsy on 16-4-1.
//

#ifndef OPENGLES_GLES2_RENDER_H_H
#define OPENGLES_GLES2_RENDER_H_H

typedef struct {
    GLuint programObject;
    GLuint mPositionHandle; //< 顶点着色器 vPosition句柄
    GLuint mColorHandle;    //< fragment着色器 vColor句柄
    int g_width;
    int g_height;
}UserContext;

GLuint LoadShader (GLenum type, const char *shaderSrc);
int userResize(UserContext *context, int width, int height);

#endif //OPENGLES_GLES2_RENDER_H_H
