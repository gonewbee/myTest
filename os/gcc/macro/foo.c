/**
 * @biref gcc使用说明
 *
 * -E: 只执行预处理，不进行编译和链接操作, gcc -E foo.c -o foo.out
 * -v: 显示编译过程，gcc -v foo.c
 *  1. 编译
 *      使用cc1进行编译，/usr/lib/gcc/x86_64-linux-gnu/4.8/cc1 -quiet -v -imultiarch x86_64-linux-gnu foo.c -quiet -dumpbase foo.c -mtune=generic -march=x86-64 -auxbase foo -version -fstack-protector -Wformat -Wformat-security -o /tmp/ccy0zD7T.s
 *  2. 汇编
 *      使用as进行汇编，as -v --64 -o /tmp/cc52pdXV.o /tmp/ccy0zD7T.s
 *  3. 链接
 *      没有直接调用ld进行链接，而是调用collect2进行链接，实际上collect2只是一个辅助程序，最终仍要调用ld完成真正的链接过程

 */
#include <stdio.h>
#include "foo.h"

int main(int argc, char* argv[]) {
    int result = 0;
    int r = 5;

#ifdef AREA
    result = PI * r * r;
#else
    result = PI *r * 2;
#endif
    fprintf(stdout, "result is: %d\n", result);
    return 0;
}

