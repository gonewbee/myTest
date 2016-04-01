//
// Created by zsy on 16-4-1.
//
///
// Create a shader object, load the shader source, and
// compile the shader.
//
#include <stdio.h>
#include <GLES2/gl2.h>
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
