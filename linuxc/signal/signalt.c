/**
 * linux信号处理
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
//#include <pthread.h>

/**
 * @brief SIGTERM信号的处理
 */
static void term_handler(int sig) {
    /* SIGTERM可以由kill命令产生 */
    if (SIGTERM==sig) {
        printf("this is SIGTERM handler\n");
    }
}

int main(int argc, char **argv) {
    printf("pid:%u\n", getpid());
//    printf("pthread tid:%lu\n", pthread_self());
    /* 捕获SIGTERM信号 */
    signal(SIGTERM, term_handler);
    sleep(30);
    return 0;
}

