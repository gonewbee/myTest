/**
 * @breif 将float数组保存到文件，在从文件中读取数据，在还原到float数组
 * 
 * void *类型用来表示这段内存数据，和数据类型无关
 * 使用libgen.h中的dirname获取上级目录
 */
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#define FILENAME    "test_data.out"

static char filename[100] = {0};

/**
 * @breif 获取文件内容长度
 */
long get_file_length(FILE *fp) {
    long size = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fprintf(stdout, "File content length:%ld\n", size);
    return size;
}

/**
 * @breif 将数据保存到文件
 */
void save2file(int size, const void *data) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    fwrite(data, 1, size, fp);
    fclose(fp);
}

void print_float_buffer(float *buf, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        if (i%8 == 0) {
            fprintf(stdout, "\n");
        }
        fprintf(stdout, "%.02f ", buf[i]);
    }
    fprintf(stdout, "\n");
}

int main(int argc, char *argv[]) {
    float data[] = {
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f 
    };
    char *dir = dirname(argv[0]);
    snprintf(filename, 100, "%s/%s", dir, FILENAME);
    fprintf(stdout, "Output filename:%s\n", filename);
    print_float_buffer(data, sizeof(data)/sizeof(float));
    fprintf(stdout, "data size:%lu\n", sizeof(data));
    save2file(sizeof(data), data);
    // 从文件中读取float类型的数组
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    long size = get_file_length(fp);
    void *readData = malloc(size);
    float *dst = (float *)readData;
    print_float_buffer(dst, size/sizeof(float));
    fread(readData, 1, size, fp);
    fclose(fp);
    print_float_buffer(dst, size/sizeof(float));
    fprintf(stdout, "address readData:%p dst:%p\n", readData, dst);
    free(readData);
    // 重复释放会导致崩溃
    // free(dst);
    return 0;
}

