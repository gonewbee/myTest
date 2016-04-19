#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int len = 100;
    int *ptr = (int *)malloc(sizeof(int)*len);
    int array[100] = {0};
    fprintf(stdout, "ptr first 4 num:%x %x %x %x\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    memset(ptr, 1, len); // 打印：ptr first 4 num:1010101 1010101 1010101 1010101
    fprintf(stdout, "ptr first 4 num:%x %x %x %x\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    memset(ptr, -1, len); // ptr first 4 num:-1 -1 -1 -1
    fprintf(stdout, "ptr first 4 num:%d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    memset(array, 1, 100); // array first 4 num:1010101 1010101 1010101 1010101
    fprintf(stdout, "array first 4 num:%x %x %x %x\n", array[0], array[1], array[2], array[3]);
    free(ptr);
    return 0;
}

