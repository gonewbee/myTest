//
// Created by zsy on 16-4-7.
//

#include <stdio.h>
#include <string.h>
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
