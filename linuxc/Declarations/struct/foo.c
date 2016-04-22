/**
 * 指针越界时不会马上崩溃
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    char b;
} FOO, *PFOO;

typedef struct {
    int a;
    char b;
    char c;
} BAR, *PBAR;

int main(int argc, char *argv[]) {
    PFOO foo = (PFOO)malloc(sizeof(FOO));
    PBAR bar = NULL;
    foo->a = 11;
    foo->b = 'a';
    bar = (PBAR)foo;
    fprintf(stdout, "bar a:%d b:%d c:%d\n", bar->a, bar->b, bar->c);
    bar->a = 22;
    bar->b = 'b';
    bar->c = 'c';
    fprintf(stdout, "foo a:%d b:%d\n", foo->a, foo->b);
    return 0;
}

