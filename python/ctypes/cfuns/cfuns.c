//
// Created by zsy on 16-4-7.
//

#include <stdio.h>
#include <string.h>
#include "cfuns.h"

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

void registerStrCallback(int(*callback)(char *)) {
    char *str = "Hello World!!";
    int ret = callback(str);
    fprintf(stdout, "len:%d\n", ret);
}
