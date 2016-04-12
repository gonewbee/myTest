/**
 * 验证restrict关键字
 *
 * 编译：gcc -std=c99 -O restrict.c
 */
#include <stdio.h>

int foo(int *a, int *b) {
    *a = 5;
    *b = 6;
    return *a + *b;
}

int rfoo(int *restrict a, int *restrict b) {
    *a = 5;
    *b = 6;
    return *a + *b;
}

int main(int argc, char *argv[]) {
    int x = 0;
    int ret = 0;
    ret = foo(&x, &x);
    fprintf(stdout, "without restrict ret:%d x:%d\n", ret, x);
    ret = rfoo(&x, &x);
    fprintf(stdout, "with restrict ret:%d x:%d\n", ret, x);
}
