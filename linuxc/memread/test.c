#include <stdio.h>

int main(int argc, char **argv) {
    unsigned int num = 0x12345678;
    printf("address:%p, num:%d\n", &num, num);
    getc(stdin);
    return 0;
}
