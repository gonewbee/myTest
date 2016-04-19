#include <stdio.h>

int main(int argc, char *argv[]) {
    // 局部变量，gdb时使用info locals命令查看，未初始化内容不固定
    int local_a[10240];
    fprintf(stdout, "local_a first 4 num: %x %x %x %x\n", local_a[0], local_a[1], local_a[2], local_a[3]);
    int local_b[10240] = {0};
    fprintf(stdout, "local_b first 4 num: %x %x %x %x\n", local_b[0], local_b[1], local_b[2], local_b[3]);
    // 第一个值为1，剩下的为0，打印1 0 0 0
    int local_c[10240] = {1};
    fprintf(stdout, "local_c first 4 num: %x %x %x %x\n", local_c[0], local_c[1], local_c[2], local_c[3]);
    return 0;
}

