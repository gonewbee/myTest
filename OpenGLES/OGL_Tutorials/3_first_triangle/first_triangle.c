/**
 * @brief 使用OpenGL ES 2.0绘制一个三角形
 * 
 * @author  zhangshy
 */
#include <stdio.h>
#include <GLES2/gl2.h> // 使用OpenGL ES 2.0
#include <GL/freeglut.h> // 提供窗口管理接口

// 用来保存vertex buffer object的句柄
static GLuint VBO;

// 在不使用shader的情况下，已经加载到buffer中的顶点位置被当作属性索引0处理
static GLuint attribute_index = 0;

static void RenderSceneCB() {
    glClear(GL_COLOR_BUFFER_BIT);

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

    glutSwapBuffers();
}

static void create_vertex_buffer() {
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

int main(int argc, char *argv[]) {
    // 初始化GLUT，可以直接从命令行传入参数如
    // '-sync': 禁止X系统天然的异步特性
    // '-gldebug': 自动检测GL的错误信息，并显示出来
    glutInit(&argc, argv);
    // 配置GLUT选项
    // GLUT_DOUBLE: 启动双缓冲（一个buffer用来绘制背景，另一个显示）
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // 创建窗口
    glutInitWindowSize(512, 384);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    // GLUT的回调函数，这个函数会连续的被GLUT的内部循环调用 
    glutDisplayFunc(RenderSceneCB);
    // 颜色RGBA四个通道，取值范围0.0-1.0 
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    create_vertex_buffer();

    // 进入GLUT的内部循环
    glutMainLoop();
    return 0;
}

