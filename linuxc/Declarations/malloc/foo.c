#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int len = 100;
    int *ptr = (int *)malloc(sizeof(int)*len);
    int array[100] = {0};
    char str[] = "Hello World!!";
    fprintf(stdout, "ptr first 4 num:%x %x %x %x\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    // 未初始化的指针空间使用strlen长度不是实际长度，sizeof返回的是指针类型的大小
    // 输出：strlen of ptr:0    sizeof of ptr:8
    fprintf(stdout, "strlen of ptr:%d\tsizeof of ptr:%d\n", strlen(ptr), sizeof(ptr));
    // memset是对字节处理的，只能用来设置0和-1
    memset(ptr, 1, len/2);
    // 打印：ptr first 4 num:1010101 1010101 1010101 1010101
    fprintf(stdout, "ptr first 4 num:%x %x %x %x\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    // 输出：strlen of ptr:100  sizeof of ptr:8
    fprintf(stdout, "strlen of ptr:%d\tsizeof of ptr:%d\n", strlen(ptr), sizeof(ptr));
    // -1实际上对应的是FF
    memset(ptr, -1, len/2);
    // ptr first 4 num:-1 -1 -1 -1
    fprintf(stdout, "ptr first 4 num:%d %d %d %d\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    memset(array, 1, 100); // array first 4 num:1010101 1010101 1010101 1010101
    fprintf(stdout, "array first 4 num:%x %x %x %x\n", array[0], array[1], array[2], array[3]);
    // 输出：strlen of char array:13    sizeof:14
    fprintf(stdout, "strlen of char array:%d\tsizeof:%d\n", strlen(str), sizeof(str));
    free(ptr);
    return 0;
}

