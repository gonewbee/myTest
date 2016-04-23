/**
 * 通过gcc -S foo.c可得到foo.s，查看foo.s可比较函数与define的区别
 */
#include <stdio.h>

#define func_sub(a, b)  ((a)-(b))

#define func_abs(a) (((a) > 0) ? (a) : -(a))

int func_sum(int a, int b) {
    return a+b;
}

int main(int argc, char *argv[]) {
    int a = 6, b = 4;
    int m = 0;
    int n = 0;
    m = func_sum(a, b);
    n = func_sub(a, b);
    fprintf(stdout, "%d %d sum:%d sub:%d\n", a, b, m, n);
    a = -4;
    // 会调用两次++a
    n = func_abs(++a);
    fprintf(stdout, "%d abs is:%d\n", a, n);
    return 0;
}

