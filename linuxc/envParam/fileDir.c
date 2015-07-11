/**
 * @brief 在目录下新建文件
 *
 * 1.stat判断目录是否存在
 * 2.mkdir创建目录
 * 3.access判断文件是否存在
 * 4.fscanf和fprintf可用于格式化的读写文件
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief 确保目录存在
 *
 * 先判断目录是否存在，不存在就创建
 * @return 0:成功 -1:失败
 */
int do_mkdir(char *dir, mode_t mode) {
    struct stat st;
    int status = 0;
    if (stat(dir, &st)!=0) {
        //目录不存在，创建目录
        if (mkdir(dir, mode)!=0) {
            perror("mkdir");
            status = -1;
        }
    } else if (!S_ISDIR(st.st_mode)){
        //不是目录
        status = -1;
    }
    return status;
}

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    int port = -1;
    if (do_mkdir("tmp", 0777)!=0) {
        fprintf(stderr, "目录创建失败\n");
        return 1;
    }
    char *fileName = "tmp/set.txt";
    if (access(fileName, 0)!=0) {
        //文件不存在
        fp = fopen(fileName, "w");
        if (!fp) {
            perror("fopen");
            return 1;
        }
        fprintf(fp, "port:[%d]\n", 12138);
    } else {
        fp = fopen(fileName, "r");
        if (!fp) {
            perror("fopen");
            return 1;
        }
        fscanf(fp, "port:[%d]\n", &port);
        fprintf(stdout, "port:[%d]\n", port);
    }
    fclose(fp);
    return 0;
}

