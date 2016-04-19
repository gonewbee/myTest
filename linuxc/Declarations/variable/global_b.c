#include <stdio.h>

// 非0的全局变量存在在.data段，编译出文件5M，占用文件空间
int global_b[10240] = {1};

int main(int argc, char *argv[]) {
    fprintf(stdout, "global_b first 4 num: %x %x %x %x\n", global_b[0], global_b[1], global_b[2], global_b[3]);
    return 0;
}

