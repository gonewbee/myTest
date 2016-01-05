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
    ret = read(fd->pipe_fd[0], buf, BUF_LEN);
    fprintf(stdout, "%lu read:%d %s\n", pthread_self(), ret, buf);
    return NULL;
}

int main(int argc, char *argv) {
    pthread_t t_id;
    int ret;
    void *status;
    char *str = "Hello World!!!";
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
    write(fds.pipe_fd[1], str, strlen(str));
    pthread_join(t_id, &status);
    close(fds.pipe_fd[0]);
    close(fds.pipe_fd[1]);
    return 0;
}
