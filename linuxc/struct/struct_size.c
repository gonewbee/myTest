/**
 * 测试struct的字节对齐问题对size的影响
 */
#include <stdio.h>
#include <stdlib.h>

// 字节对齐影响StructA大小为8
typedef struct {
    int a;
    char c;
} StructA, *PStructA;

#pragma pack(push) // 保存对齐状态
#pragma pack(1) // 设定为1字节对齐
// 设定为1字节对齐后，StructP大小为实际大小5
typedef struct {
    int a;
    char c;
} StructP, *PStructP;
#pragma pack(pop) // 恢复对齐状态

// __attribute__((packed))取消对齐
typedef struct {
    int a;
    char c;
} __attribute__((packed)) StructE, *PStructE;

// 使用__attribute__((aligned(n)))设置n字节对齐
// **如果结构体中有成员长度大于n，则按照最大成员长度来对齐
typedef struct {
    __attribute__((aligned(8))) int a;
    __attribute__((aligned(8))) char c;
}  StructF, *PStructF;

// StructB大小为12
typedef struct {
    int a;
    char c;
    int d;
} StructB, *PStructB;


// sizeof为2
typedef struct {
    char x;
    char y;
} StructX, PStructX;

// 因为字节对齐大小为16
typedef struct {
    long l;
    char c;
} StructL, PStructL;

int main(int argc, char *argv[]) {
    int lena = sizeof(StructA); // 8
    int lenb = sizeof(StructB); // 12
    int lenp = sizeof(StructP); // 5
    int lene = sizeof(StructE); // 5
    int lenf = sizeof(StructF); // 16
    int lenx = sizeof(StructX); // 2
    int lenl = sizeof(StructL); // 16
    PStructA testa = NULL;
    fprintf(stdout, "sizeof StructA is:%d\n", lena);
    fprintf(stdout, "sizeof StructB is:%d\n", lenb);
    fprintf(stdout, "sizeof StructP is:%d\n", lenp);
    fprintf(stdout, "sizeof StructE is:%d\n", lene);
    fprintf(stdout, "sizeof StructF is:%d\n", lenf);
    fprintf(stdout, "sizeof StructX is:%d\n", lenx);
    fprintf(stdout, "sizeof StructL is:%d\n", lenl);
    testa = (PStructA)malloc(lena);
    testa->a = 100;
    testa->c = 'c';
    free(testa);
    return 0;
}

