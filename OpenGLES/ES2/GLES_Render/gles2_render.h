//
// Created by zsy on 16-4-1.
//

#ifndef OPENGLES_GLES2_RENDER_H_H
#define OPENGLES_GLES2_RENDER_H_H

typedef struct {
    GLuint programObject;
    GLuint mPositionHandle; //< 顶点着色器 vPosition句柄
    GLuint mColorHandle;    //< fragment着色器 vColor句柄
    GLuint texCoordLoc;     //< 纹理坐标的attribute location
    GLuint textureMapLoc;   //< 纹理的sampler location
    GLuint textureMapTexId;
    int g_width;
    int g_height;
}UserContext;

GLuint LoadShader (GLenum type, const char *shaderSrc);
int userResize(UserContext *context, int width, int height);

/**
 * @brief 从tga格式文件中加载纹理
 */
GLuint LoadTexture (UserContext *context, char *fileName);

#endif //OPENGLES_GLES2_RENDER_H_H
