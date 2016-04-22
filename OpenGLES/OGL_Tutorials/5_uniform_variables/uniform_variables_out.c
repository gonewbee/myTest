/**
 * @brief 使用OpenGL ES 2.0绘制一个三角形
 * 
 * @author  zhangshy
 */
#include <stdio.h>
#include <GLES2/gl2.h> // 使用OpenGL ES 2.0
#include <GL/freeglut.h> // 提供窗口管理接口
#include "uniform_variables.h"

static void RenderSceneCB() {
    draw();
    glutSwapBuffers();
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
    // 加上这句之后显示了动态的效果？？
    glutIdleFunc(RenderSceneCB);
    // 颜色RGBA四个通道，取值范围0.0-1.0 
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    compile_shaders();
    create_vertex_buffer();

    // 进入GLUT的内部循环
    glutMainLoop();
    return 0;
}

