/**
 * @brief 使用c生成稀疏文件(Sparse File)，文件黑洞(file hole)
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * 使用fseek可以产生文件黑洞
 */
void fseek_size(FILE *f, long size) {
    fseek(f, size, SEEK_SET);
    fputc('\0', f);
}

/**
 * 使用ftruncate可以产生长度为size，但占用磁盘空间为0的文件，通过stat命令查看
 */
void ftruncate_size(FILE *f, long size) {
    ftruncate(fileno(f), size);
}

int main(int argc, char *argv[]) {
    FILE *f;
    long size = 0x80;
    if (argc>1) {
        f = fopen(argv[1], "w+");
    } else {
        f = fopen("test.out", "w+");
    }
    if (!f) {
        perror("open file!");
        return -1;
    }

#if 0
    fseek_size(f, size);
#else
    ftruncate_size(f, size);
#endif
    fclose(f);
    return 0;
}

