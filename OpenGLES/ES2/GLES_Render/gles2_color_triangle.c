//
// Created by zsy on 16-4-5.
//

#include <stdio.h>
#include <GLES2/gl2.h>
#include "gles2_color_triangle.h"

static GLuint VBO_vertices_len;
static GLuint VBO_vertex_len;
static GLuint VBO_color_len;

void setup_color_triangle_VBOs(UserContext *context) {
    GLfloat vertices[] = {
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Vertex 1: Red
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Vertex 2: Green
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f  // Vertex 3: Blue
    };
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    VBO_vertices_len = 7;
    VBO_vertex_len = 3;
    VBO_color_len = 4;
};

int color_triangle_create(UserContext *context) {
    // varying变量提供Vertex Shader和Fragment Shader之间的接口
    GLbyte vShaderStr[] =
            "attribute vec4 position;   \n"
            "attribute vec4 color;      \n"
            "varying vec4 v_color;      \n"
            "void main() {              \n"
            "   v_color = color;        \n"
            "   gl_Position = position; \n"
            "}                          \n";
    // 不加lowp的话glCompileShader失败
    GLbyte fShaderStr[] =
            "varying lowp vec4 v_color; \n"
            "void main() {              \n"
            "   gl_FragColor = v_color; \n"
            "}                          \n";

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

    // Link the program
    // 创建OpenGL ES的可执行程序
    glLinkProgram ( context->programObject );

    // Check the link status
    glGetProgramiv ( context->programObject, GL_LINK_STATUS, &linked );

    if ( !linked ) {
        glDeleteProgram ( context->programObject );
        return 0;
    }

    context->mPositionHandle = glGetAttribLocation(context->programObject, "position");
    context->mColorHandle = glGetAttribLocation(context->programObject, "color");
    glEnableVertexAttribArray(context->mPositionHandle);
    glEnableVertexAttribArray(context->mColorHandle);

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    return 1;
}

int color_triangle_draw(UserContext *context) {
    glViewport ( 0, 0, context->g_width, context->g_height );
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(context->programObject);

    glVertexAttribPointer(context->mPositionHandle, VBO_vertex_len, GL_FLOAT, GL_FALSE,
                          VBO_vertices_len* sizeof(GLfloat), 0);
    glVertexAttribPointer(context->mColorHandle, VBO_color_len, GL_FLOAT, GL_FALSE,
                          VBO_vertices_len* sizeof(GLfloat), (void*)(VBO_vertex_len*sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    return 1;
}
