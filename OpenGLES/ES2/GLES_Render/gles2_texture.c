//
// Created by zsy on 16-4-6.
//

#include <stdio.h>
#include <GLES2/gl2.h>
#include "gles2_texture.h"
#include "gles2_render.h"

static GLuint VBO_vertices_len;
static GLuint VBO_vertex_len;
static GLuint VBO_color_len;
static GLuint VBO_texcoords_len;
static GLuint elements_len;

void setup_texture_VBOs(UserContext *context) {
    GLfloat vVertices[] = {
    //  Position            Color                   Texcoords
        -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
        0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Top-right
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };
    GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
    };

    VBO_vertices_len = 9;
    VBO_vertex_len = 3;
    VBO_color_len = 4;
    VBO_texcoords_len = 2;
    elements_len = sizeof(elements)/ sizeof(elements[0]);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

int texture_create(UserContext *context) {
    GLbyte vShaderStr[] =
            "attribute vec4 position;   \n"
            "attribute vec4 color;      \n"
            "attribute vec2 texcoord;   \n"
            "varying vec4 v_color;      \n"
            "varying vec2 v_texcoord;   \n"
            "void main() {              \n"
            "   v_color = color;        \n"
            "   v_texcoord = texcoord;  \n"
            "   gl_Position = position; \n"
            "}";
    GLbyte fShaderStr[] =
            "varying lowp vec4 v_color;"
            "varying lowp vec2 v_texcoord;"
            "uniform sampler2D texture;"
            "void main() {"
            "   gl_FragColor = v_color * texture2D(texture, v_texcoord);"
            "}";

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
    context->texCoordLoc = glGetAttribLocation(context->programObject, "texcoord");
    context->textureMapLoc = glGetUniformLocation(context->programObject, "texture");
    glEnableVertexAttribArray(context->mPositionHandle);
    glEnableVertexAttribArray(context->mColorHandle);
    glEnableVertexAttribArray(context->texCoordLoc);

    glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
    return 1;
}

int texture_draw(UserContext *context) {
    glViewport ( 0, 0, context->g_width, context->g_height );
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(context->programObject);

    glVertexAttribPointer(context->mPositionHandle, VBO_vertex_len, GL_FLOAT, GL_FALSE,
                          VBO_vertices_len* sizeof(GLfloat), 0);
    glVertexAttribPointer(context->mColorHandle, VBO_color_len, GL_FLOAT, GL_FALSE,
                          VBO_vertices_len* sizeof(GLfloat), (void*)(VBO_vertex_len*sizeof(GLfloat)));
    glVertexAttribPointer(context->texCoordLoc, VBO_texcoords_len, GL_FLOAT, GL_FALSE,
                          VBO_vertices_len* sizeof(GLfloat), (void*)((VBO_vertex_len+VBO_color_len)*sizeof(GLfloat)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, context->textureMapTexId);
    glUniform1i(context->textureMapLoc, 0);

    glDrawElements(GL_TRIANGLES, elements_len, GL_UNSIGNED_INT, 0);
    return 1;
}
