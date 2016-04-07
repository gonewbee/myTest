//
// Created by zsy on 16-4-7.
//

#ifndef CTYPES_CFUNS_H
#define CTYPES_CFUNS_H

typedef int (*pCallback)(char *);

int add(int a, int b);
int getLen(char *str);
void hello(char *str);

//void registerStrCallback(int(*callback)(char *));
void registerStrCallback(pCallback callback);
void runCallback();

#endif //CTYPES_CFUNS_H
