#include <stdio.h>

int add(int a, int b) {
    int m = 0x34;
    int n = 0x45;
    int c = a+b;
    if (c>0x20)
        c += m;
    else
        c += n;
    return c;
}

int log1(int a) {
    int r = a+0x12;
    fprintf(stdout, "%s ret:::%x\n", __func__, r);
    return 1;
}

int main(int argc, char *argv[]) {
    int a = 0x10;
    int b = 0x11;
    int d;
    d = add(a,b);
    log1(d);
    return 0;
}
