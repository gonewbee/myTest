/**
 * 通过objdump -t 可列出符号表，可查看到static类型变量的数据段 
$ objdump -t local_static | grep local
local_static:     file format elf64-x86-64
0000000000000000 l    df *ABS*  0000000000000000              local_static.c
000000000060b080 l     O .bss   000000000000a000              local_a.2180
0000000000615080 l     O .bss   000000000000a000              local_b.2181
0000000000601060 l     O .data  000000000000a000              local_c.2182
 */
#include <stdio.h>

int main(int argc, char *argv[]) {
    // 未初始化static类型局部变量，在.bss段，不占用文件空间
    static int local_a[10240];
    fprintf(stdout, "local_a first 4 num: %x %x %x %x\n", local_a[0], local_a[1], local_a[2], local_a[3]);
    // 全部初始化为0的static类型局部变量也在.bss段
    static int local_b[10240] = {0};
    fprintf(stdout, "local_b first 4 num: %x %x %x %x\n", local_b[0], local_b[1], local_b[2], local_b[3]);
    // 不是全0的static类型变量存在在.data段，占用文件空间，文件变大
    static int local_c[10240] = {1};
    fprintf(stdout, "local_c first 4 num: %x %x %x %x\n", local_c[0], local_c[1], local_c[2], local_c[3]);
    return 0;
}

