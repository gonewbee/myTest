/**
 * @file test_cond.c
 * @brief 使用pthread_cond_timedwait实现等待超时功能
 * @attention 编译gcc test_cond.c -lpthread
 */
#include <stdio.h>
#include <pthread.h>

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_test_func(void *arg) {
    int time = (int)arg;
    printf("%u enter\n", pthread_self());
    printf("try to wait:%d\n", time);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("1 sec:%d\n", ts.tv_sec);
    ts.tv_sec += time;
    pthread_mutex_lock(&mutex);
    pthread_cond_timedwait(&cond, &mutex, &ts);
    pthread_mutex_unlock(&mutex);
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("2 sec:%d\n", ts.tv_sec);
    printf("%u end\n", pthread_self());
}

int main(int argc, char **argv) {
    pthread_t pid;
    int time = 10;
    pthread_create(&pid, NULL, thread_test_func, (void *)time);
    printf("pid:%d\n", pid);
#if 1
    sleep(1);
    pthread_mutex_lock(&mutex);
    printf("%s %d\n", __func__, __LINE__);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
#endif
    pthread_join(pid, NULL);
    printf("%s %s end\n", __FILE__, __func__);
}

