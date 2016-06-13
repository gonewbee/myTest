/**
 * @brief 测试__attribute__((constructor))和__attribute__((destructor))属性
 */
#include <stdio.h>

/* __attribute__((constructor))装饰的函数在main运行前执行  */
void startFunc(void) __attribute__((constructor));

void startFunc(void) {
    fprintf(stdout, "%s enter!\n", __func__);
}

/* __attribute__((destructor))装饰的函数在在main结束后执行 */
void __attribute__((destructor)) cleanFunc(void) {
    fprintf(stdout, "%s enter!\n", __func__);
}

int main(int argc, char *argv[]) {
    fprintf(stdout, "main run!!!!!\n");
    return 0;
}

