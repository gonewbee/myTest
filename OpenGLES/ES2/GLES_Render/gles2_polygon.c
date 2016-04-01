//
// Created by zsy on 16-4-1.
//
#include <stdio.h>
#include <GLES2/gl2.h>
#include "gles2_render.h"

int polygon_create(UserContext *context) {
    // Vertex shader源码，设置坐标，对应attribute操作
    GLbyte vShaderStr[] =
            "attribute vec4 vPosition;    \n"
            "void main()                  \n"
            "{                            \n"
            "   gl_Position = vPosition;  \n"
            "}                            \n";

    // Fragment shader源码，设置颜色，对应uniform操作
    GLbyte fShaderStr[] =
            "precision mediump float; \n"
            "uniform vec4 vColor;     \n"
            "void main()              \n"
            "{                        \n"
            "  gl_FragColor = vColor; \n"
            "}                        \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;
    // Load the vertex/fragment shaders
    // Vertex Shader 形状的顶点
    vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
    // Fragment Shader 形状的颜色或纹理
    fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Program 是OpenGL ES的对象，该对象包含用来绘制一个或多个形状的着色器(shader)
    // Create the program object
    context->programObject = glCreateProgram ( );
    if ( context->programObject == 0 ) {
        return 0;
    }
    // 将顶点着色器添加到program对象
    glAttachShader ( context->programObject, vertexShader );
    glAttachShader ( context->programObject, fragmentShader );

    context->mPositionHandle = 0;

    // Bind vPosition to attribute mPositionHandle
    // 绑定vPosition到mPositionHandle
    glBindAttribLocation ( context->programObject, context->mPositionHandle, "vPosition" );

    // Link the program
    // 创建OpenGL ES的可执行程序
    glLinkProgram ( context->programObject );

    // Check the link status
    glGetProgramiv ( context->programObject, GL_LINK_STATUS, &linked );

    if ( !linked ) {
        glDeleteProgram ( context->programObject );
        return 0;
    }
    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    return 1;
}

int polygon_draw(UserContext *context) {
    GLfloat vVertices[] = {  // in counterclockwise order 逆时针顺序
            -0.5f,  0.5f, 0.0f,     // top left
            -0.5f, -0.5f, 0.0f,     // bottom left
            0.5f, -0.5f, 0.0f,      // bottom right
            0.5f, 0.5f, 0.0f        // bottom right
    };
    // glDrawElements绘图时的顺序
    GLuint  vIndices[] = {
            0, 1, 2,
            0, 2, 3
    };
    // 颜色
    GLfloat vColors[] = {0.0, 1.0, 1.0, 1.0};
    // 顶点的坐标纬数，(x, y, z)坐标纬数为3
    int coords_per_vertex = 3;
    // 计算顶点数目
    int vertexCount = sizeof(vVertices)/(sizeof(GLfloat)*3);
//    fprintf(stdout, "%s vertexCount:%d\n", __func__, vertexCount);
    int indicesCount = sizeof(vIndices)/ sizeof(GLfloat);

    // Set the viewport
    glViewport ( 0, 0, context->g_width, context->g_height );

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( context->programObject );

    // Load the vertex data
    glVertexAttribPointer ( context->mPositionHandle, coords_per_vertex, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( context->mPositionHandle );

    // 获取fragment shader的vColor
    context->mColorHandle = glGetUniformLocation(context->programObject, "vColor");
    // fprintf(stdout, "%s mColorHandle:%d\n", __func__, mColorHandle);
    glUniform4fv(context->mColorHandle, 1, vColors);

//    glDrawArrays ( GL_TRIANGLES, 0, vertexCount );
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, vIndices);
    return 1;
}
