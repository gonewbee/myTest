/**
 * @brief 比较使用字节对齐和不使用字节对齐时操作耗时
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int a;
    char b;
    int c;
    char d;
    int e;
    char f;
    int m;
    char n;
    int k;
    char x;
    int y;
    char z;
    int g;
} FOO, *PFOO;

// __attribute__((packed))取消对齐
typedef struct {
    int a;
    char b;
    int c;
    char d;
    int e;
    char f;
    int m;
    char n;
    int k;
    char x;
    int y;
    char z;
    int g;
} __attribute__((packed)) BAR, *PBAR;

int main(int argc, char *argv[]) {
    PFOO foo = (PFOO)malloc(sizeof(FOO));
    PBAR bar = (PBAR)malloc(sizeof(BAR));
    int i = 0, j = 0;
    int n = 50000;

    clock_t t = clock();
    for (i=0; i<n; i++) {
        for (j = 0; j<n; j++) {
            foo->g = j;
        }
    }
    fprintf(stdout, "With alignment sizeof:%lu. Time used:%.2f Seconds.\n", sizeof(FOO), (double)(clock() - t)/CLOCKS_PER_SEC);
    
    t = clock();
    for (i=0; i<n; i++) {
        for (j = 0; j<n; j++) {
            bar->g = j;
        }
    }
    fprintf(stdout, "Without alignment sizeof:%lu. Time used:%.2f Seconds.\n", sizeof(BAR), (double)(clock() - t)/CLOCKS_PER_SEC);
    return 0;
}

