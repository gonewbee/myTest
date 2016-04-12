//
// Created by zsy on 16-4-7.
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cfuns.h"

static pCallback g_callback;

int add(int a, int b) {
    int c = a + b;
    fprintf(stdout, "%d+%d=%d\n", a, b, c);
    return c;
}

int getLen(char *str) {
    int len = strlen(str);
    fprintf(stdout, "%s input:%s\n", __func__, str);
    return len;
}

void hello(char *str) {
    fprintf(stdout, "hello %s!\n", str);
}

int cstrargs(int n, char *str, ...) {
    va_list argptr;
    int num = 0;
    va_start(argptr, str);
    char *t = str;
    num += 1;
    fprintf(stdout, "n:%d\n", n);
    fprintf(stdout, "%s\n", t);
//    while ((t=va_arg(argptr, char*)) != 0) {
//        fprintf(stdout, "%s\n", t);
//        num += 1;
//    }
    while (num < n) {
        t = va_arg(argptr, char *);
        fprintf(stdout, "%s\n", t);
        num += 1;
    }
    va_end(argptr);
    return num;
}

int arrtest(int n, char *p[]) {
    int i;
    for (i=0; i<n; i++) {
        fprintf(stdout, "%s\n", p[i]);
        sprintf(p[i], "re%d", i);
    }
    return 0;
}

void registerStrCallback(pCallback callback) {
    g_callback = callback;
}

void runCallback() {
    char str[16];
    int i = 0;
    int ret;
    for (i=0; i<100; i+=10) {
        memset(str, 0, 16);
        snprintf(str, 16, "callback %d", i);
        ret = g_callback(str);
        fprintf(stdout, "%s ret:%d\n", str, ret);
    }
}
