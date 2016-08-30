#include <stdio.h>

/* 联合体大小为最大的变量的大小 */
union U1 {
    int i;
    short s;
    char c;
    char a[4];
};

/* 结构体的大小为各个变量大小的和，还要考虑字节对齐 */
struct S1 {
    int i;
    short s;
    char c;
};

int main(int argc, char *argv[]) {
    printf("sizeof U1:%d\n", sizeof(union U1));
    printf("sizeof S1:%d\n", sizeof(struct S1));
    union U1 u1;
    u1.i = 0x12345678;
    printf("in U1 i:0x%x s:0x%x c:0x%x a:0x%x 0x%x 0x%x 0x%x\n", u1.i, u1.s, u1.c, u1.a[0], u1.a[1], u1.a[2], u1.a[3]);
    int i = 1;
    char *c = (char *)&i;
    if (*c==1) {
        printf("Little endian\n");
    } else {
        printf("Big endian\n");
    }
    return 0;
}

