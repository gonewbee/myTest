/**
 * @file test_join.c
 * @brief 测试线程sleep和join
 * @attention 编译gcc test_join.c -lpthread
 */
#include <stdio.h>
#include <pthread.h>

static void *thread_sleep_test(void *arg) {
    printf("%u enter\n", pthread_self());
    int time = *(int *)arg;
    printf("try to sleep:%d\n", time);
    sleep(time);
    printf("%u end\n", pthread_self());
}

int main(int argc, char **argv) {
    pthread_t pid;
    int time = 5;
    int ret = pthread_create(&pid, NULL, thread_sleep_test, (void *)(&time));
    pthread_join(pid, NULL);
    printf("main end\n");
}

