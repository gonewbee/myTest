/**
 * @brief linux下使用pipe进行进程间通信
 *
 * 编译：gcc pipe_pthread.c -lpthread
 */
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUF_LEN 128

typedef struct {
    int id;
    int pipe_fd[2];
} TEST_PIPE;

void *thread_func(void *arg) {
    int ret = 0;
    char buf[BUF_LEN] = {0};
    TEST_PIPE *fd = (TEST_PIPE *)arg;
    fprintf(stdout, "id is:%d\n", fd->id);
    while ((ret = read(fd->pipe_fd[0], buf, BUF_LEN))>0) {
        fprintf(stdout, "%lu read:%d %s\n", pthread_self(), ret, buf);
        memset(buf, 0, BUF_LEN);
    }
    fprintf(stdout, "%lu ret:%d\n", pthread_self(), ret);
    return NULL;
}

int main(int argc, char *argv) {
    pthread_t t_id;
    int ret, i;
    void *status;
    char str[] = "Hello World!!!";
    TEST_PIPE fds;
    fds.id = 123;
    if (pipe(fds.pipe_fd)<0) {
        perror("pipe");
        return -1;
    }
    ret = pthread_create(&t_id, NULL, thread_func, (void *)(&fds));
    if (ret!=0) {
        fprintf(stderr, "pthread_create error::%d\n", ret);
        return -1;
    }
    fprintf(stdout, "main try to write!\n");
    for (i=0; i<6; i++) {
        write(fds.pipe_fd[1], str, i);
        sleep(1);
    }
    close(fds.pipe_fd[0]);
    close(fds.pipe_fd[1]);
    pthread_join(t_id, &status);
    return 0;
}
