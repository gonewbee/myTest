//
// Created by zsy on 16-4-1.
//
///
// Create a shader object, load the shader source, and
// compile the shader.
//
#include <stdio.h>
#include <GLES2/gl2.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "gles2_render.h"

GLuint LoadShader (GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
        return 0;

    // Load the shader source
    // 加载shader源码
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    // 编译shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled ) {
        GLint infoLen = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = malloc (sizeof(char) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            fprintf(stderr,  "Error compiling shader:\n%s\n", infoLog);

            free ( infoLog );
        }
        glDeleteShader ( shader );
        return 0;
    }

    return shader;
}

int userResize(UserContext *context, int width, int height) {
    context->g_width = width;
    context->g_height = height;
    return 1;
}

GLuint LoadTexture (UserContext *context, char *fileName) {
    FILE *f;
    unsigned char tgaheader[12];
    unsigned char attributes[6];
    int width, height;
    unsigned int imageSize;
    GLubyte *imageData;
    GLubyte temp;
    GLuint bytesPerPixel; // 像素点位宽
    GLuint type = GL_RGBA;
    GLuint texId = 0;
    GLuint i=0;
    int targetNum, readNum;

    f = fopen(fileName, "rb");
    if (NULL == f) {
        fprintf(stderr, "file:%s error:%s\n", fileName, strerror(errno));
        return texId;
    }

    targetNum = sizeof(tgaheader);
    if ((readNum=fread(tgaheader, 1, targetNum, f)) != targetNum) {
        fprintf(stderr, "%s %d read:%d of %d\n", __func__, __LINE__, readNum, targetNum);
        goto fail_fclose;
    }

    targetNum = sizeof(attributes);
    if ((readNum=fread(attributes, 1, targetNum, f)) != targetNum) {
        fprintf(stderr, "%s %d read:%d of %d\n", __func__, __LINE__, readNum, targetNum);
        goto fail_fclose;
    }

    width = attributes[1] * 256 + attributes[0];
    height = attributes[3] * 256 + attributes[2];
    bytesPerPixel = attributes[4] / 8;
    imageSize = bytesPerPixel * width * height;
    imageData = (GLubyte *)malloc(imageSize);
    if (NULL == imageData) {
        fprintf(stderr, strerror(errno));
        goto fail_fclose;
    }

    targetNum = imageSize;
    if ((readNum=fread(imageData, 1, targetNum, f)) != targetNum) {
        fprintf(stderr, "%s %d read:%d of %d\n", __func__, __LINE__, readNum, targetNum);
        goto fail_free;
    }

    fprintf(stdout, "bytesPerPixel:%d bits:%d\n", bytesPerPixel, attributes[4]);
    // 交换颜色顺序
    for (i=0; i < imageSize; i+=bytesPerPixel) {
        temp = imageData[i];
        imageData[i] = imageData[i+2];
        imageData[i+2] = temp;
    }

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    if (3 == bytesPerPixel) {
        type = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

fail_free:
    free(imageData);
fail_fclose:
    fclose(f);
    context->textureMapTexId = texId;
    return texId;
}
