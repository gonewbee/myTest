/**
 * @brief 使用有名管道进行进程见通信
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
//#include <sys/select.h>

#define P_FIFO "/tmp/xfreerdp_pipe"

int main(int argc, char **argv) {
    int fd = -1;
    if (access(P_FIFO, F_OK)==0) {
        /* 管道存在，非阻塞方式打开，写方式 */
        fd = open(P_FIFO, O_WRONLY|O_NONBLOCK);
        if (fd==-1) {
            perror("FIFO open for write");
            if (errno==ENXIO) {
                /* 没有应用读这个管道 */
                printf("no read fifo\n");
                goto fifo_read;
            }
            return -1;
        }
        if (argc>1) {
            write(fd, argv[1], sizeof(argv[1]));
        } else {
            write(fd, argv[0], sizeof(argv[0]));
        }
        close(fd);
        return 0;
    }

    /* 管道不存在，新建管道 */
    if (mkfifo(P_FIFO, 0777)==-1) {
        perror("mkfifo");
        return -1;
    }

fifo_read:
    fd = open(P_FIFO, O_RDONLY|O_NONBLOCK);
    if (fd==-1) {
        perror("FIFO open for read");
        return -1;
    }
    char buf[40] = {0};
    int len = 0;
#if 0
    fd_set rfds_set;
    struct timeval timeout;
    int status = 0;
#endif
    while (1) {
#if 0
        FD_ZERO(&rfds_set);
        FD_SET(fd, &rfds_set);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        status = select(fd+1, &rfds_set, NULL, NULL, &timeout);
        if (status==0) {
            continue;
        }
        if (status==-1) {
            perror("select");
            break;
        }
#endif
        len = read(fd, buf, 40);
        if (len<0) {
            perror("read");
            close(fd);
            return -1;
        }
        if (len==0) {
            printf("no data\n");
        } else {
            printf("read:%s\n", buf);
            memset(buf, 0, len);
        }
        sleep(1);
    }
    close(fd);
    return 0;
}

