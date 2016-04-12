//
// Created by zsy on 16-4-7.
//

#ifndef CTYPES_CFUNS_H
#define CTYPES_CFUNS_H

typedef int (*pCallback)(char *);

int add(int a, int b);
int getLen(char *str);
void hello(char *str);
int cstrargs(int n, char *str, ...);
int arrtest(int n, char *p[]);

//void registerStrCallback(int(*callback)(char *));
void registerStrCallback(pCallback callback);
void runCallback();

const char *str_return();

#endif //CTYPES_CFUNS_H
