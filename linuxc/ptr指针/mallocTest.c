/**
 * @file mallocTest.c
 * @brief 测试malloc，将地址作为参数传递并保存。
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char ch;
    int value;
    int no;
}_my_var;

static char gNo = 0;

/**
 * @brief 申请的变量其实是结构体，但只是把地址当作参数传递出来，使用结束后记得释放
 */
static void my_alloc(long *v) {
    _my_var *tmp = (_my_var *)malloc(sizeof(_my_var));
    tmp->no = gNo;
    tmp->value = gNo*gNo;
    tmp->ch = gNo+'A';
    gNo++;
    *v = tmp;
    printf("malloc:%u v:%u\n", tmp, *v);
}

static void my_free(long *v) {
    _my_var *tmp = (_my_var *)(*v);
    free(tmp);
}

static void print_var(long *v) {
    _my_var *tmp = (_my_var *)(*v);
    printf("%u %d %d %c\n", *v, tmp->no, tmp->value, tmp->ch);
}

int main(int argc, char **argv) {
    int i = 0;
    long buf[10];
    for (i=0; i<10; i++) {
        my_alloc(&buf[i]);
    }
    for (i=0; i<10; i++) {
        print_var(&buf[i]);
    }
    for (i=0; i<10; i++) {
        my_free(&buf[i]);
    }
}

