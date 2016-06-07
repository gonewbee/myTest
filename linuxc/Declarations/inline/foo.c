#include <stdio.h>
#include <time.h>

inline int inline_add(int a, int b) {
    return a+b;
}

int func_add(int a, int b) {
    return a+b;
}

int main(int argc, char *argv[]) {
    clock_t t = 0;
    int i = 0;
    long n = 0;
    n = 100000000;
    t = clock();
    for (i=0; i<n; i++) {
        inline_add(i, i+1);
    }
    fprintf(stdout, "Call inline function %lu times. Time:%.2f seconds\n",
            n, (double)(clock() - t)/CLOCKS_PER_SEC);
    t = clock();
    for (i=0; i<n; i++) {
        func_add(i, i+1);
    }
    fprintf(stdout, "Call function %lu times. Time:%.2f seconds\n",
            n, (double)(clock() - t)/CLOCKS_PER_SEC);
    return 0;
}

