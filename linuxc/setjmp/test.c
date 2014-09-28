/**
 * setjmp longjmp用于程序跳转
 *
 * 测试：当输入命令行参数argc大于1时，执行longjmp
 */
#include <stdio.h>
#include <setjmp.h> 

static jmp_buf buf;

void dealInput(int argc) {
    printf("%s func in\n", __func__);
    if (argc!=1)
        longjmp(buf, argc);
    printf("%s func end\n", __func__);
}

int main(int argc, char **argv) {
    int ret=0;
    printf("argc:%d\n", argc);
    if (0==(ret=setjmp(buf))) {
        printf("setjmp返回0, 自动执行到setjmp\n");
        dealInput(argc);
    } else {
        printf("setjmp返回%d, 通过longjmp调用\n", ret);
    }
    printf("main function end\n");
}
