/**
 * @brief 通过uniform变换(x, y)坐标，进而改变三角形尺寸
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <GLES2/gl2.h>

// 用来保存vertex buffer object的句柄
static GLuint VBO;

// 在不使用shader的情况下，已经加载到buffer中的顶点位置被当作属性索引0处理
static GLuint attribute_index = 0;

// 用户不能直接使用shader的内容也不能直接修改它的变量，
// 当编译完shader后，GLSL编译器为每一个uniform变量提供了一个索引，
// 应用可以通过glGetUniformLocation获取这个索引，如果错误返回-1。
static GLuint gScaleLocation;

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;
    Scale += 0.001f;
    // 通过sin传递-1.0到1.0直接的值
    glUniform1f(gScaleLocation, sin(Scale));

    glEnableVertexAttribArray(attribute_index);
    // OpenGL使用一种相当独特的方法来使用这些句柄。
    // 在许多API中简单的将句柄传给相关函数，操作就会发生在那些句柄上
    // 在OpenGL我们将句柄绑定到一个目标的名字，在这个目标上执行命令，这些命令
    // 会影响绑定到它的句柄，直到另一个句柄代替这个句柄
    // 目标GL_ARRAY_BUFFER表示buffer存储的是顶点数组
    // 另一个常用目标GL_ELEMENT_ARRAY_BUFFER表示这个buffer存储的是顶点的indices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 告诉管道如何解释buffer中的数据
    // attribute_index：表示属性的索引，当前代表的是顶点位置
    // 3：属性的成分的个数，3代表X、Y、Z
    // GL_FLOAT：表示属性的成分的类型
    // GL_FALSE：表示是否要将属性在管道中使用之前normalize
    // 0：幅度，buffer中两个属性实例间隔的字节数
    // 0：偏移量
    glVertexAttribPointer(attribute_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(attribute_index);
}

void create_vertex_buffer() {
    // 三角形的三个点
    GLfloat vVertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
    // OpenGL定义数个glGen*函数来生成不同类型的对象
    // 大部分需要两个参数
    // 第一个参数：要创建的对象的数目
    // 第二个参数：存储创建的句柄数组的地址，空间要够大
    glGenBuffers(1, &VBO);
    // OpenGL使用一种相当独特的方法来使用这些句柄。
    // 在许多API中简单的将句柄传给相关函数，操作就会发生在那些句柄上
    // 在OpenGL我们将句柄绑定到一个目标的名字，在这个目标上执行命令，这些命令
    // 会影响绑定到它的句柄，直到另一个句柄代替这个句柄
    // 目标GL_ARRAY_BUFFER表示buffer存储的是顶点数组
    // 另一个常用目标GL_ELEMENT_ARRAY_BUFFER表示这个buffer存储的是顶点的indices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 绑定完对象后向它填充数据，glBufferData使用刚刚绑定的目标名称
    // GL_STATIC_DRAW表示数据的使用模式为不修改buffer的内容。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
}

void load_shader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType) {
    // 创建shader对象，我们将会创建GL_VERTEX_SHADER和GL_FRAGMENT_SHADER
    GLuint shaderObj = glCreateShader(shaderType);
    // 加载shader源码
    glShaderSource(shaderObj, 1, &pShaderText, NULL);
    // 编译shader
    glCompileShader(shaderObj);
    // 判断是否编译成功
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint infoLen = 0;
        glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            // 获取错误信息
            glGetShaderInfoLog(shaderObj, infoLen, NULL, infoLog);
            fprintf(stderr, "Error compiling shader type:%d:\n%s\n", shaderType, infoLog);
            free(infoLog);
        }
        glDeleteShader(shaderObj);
        exit(EXIT_FAILURE);
    }
    // 将编译好的shader绑定到shaderProgram
    glAttachShader(shaderProgram, shaderObj);
}

void compile_shaders() {
    char vShaderStr[] = 
        "#version 100\n"
        "attribute vec3 Position;"
        "uniform float gScale;"
        "void main() {"
        "   gl_Position = vec4(gScale*Position.x, gScale*Position.y, Position.z, 1.0);"
        "}";
    char fShaderStr[] = 
        "#version 100\n"
        "precision mediump float;"
        "void main() {"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";
    // 将所有shader链接到这个对象
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        fprintf(stdout, "Error creating shader program!\n");
        exit(EXIT_FAILURE);
    }
    load_shader(shaderProgram, vShaderStr, GL_VERTEX_SHADER);
    load_shader(shaderProgram, fShaderStr, GL_FRAGMENT_SHADER);
    // 链接。
    // 链接后可使用glDetachShader和glDeleteShader解除
    glLinkProgram(shaderProgram);
    GLint success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar error[1024] = {0};
        glGetProgramInfoLog(shaderProgram, sizeof(error), NULL, error);
        fprintf(stderr, "Error linking shader program:%s\n", error);
        exit(EXIT_FAILURE);
    }
    // 最后使用这个链接了shader的program，这个program一直有效，直到被另一个替换或
    // 通过glUseProgram(NULL)停止使用
    glUseProgram(shaderProgram);
    // 链接后，从program对象中获取uniform变量的位置
    gScaleLocation = glGetUniformLocation(shaderProgram, "gScale");
    assert(gScaleLocation != 0xFFFFFFFF);
}

