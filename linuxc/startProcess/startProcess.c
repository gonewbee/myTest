/**
 * @brief 测试打开另一个应用
 *
 * 编译：gcc startProcess.c -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @brief 使用system启动另一个应用
 *
 * 通过system会阻塞，等待命令执行结束
 */
void system_call() {
    int status = system("/home/zsy/Workspace/down/FreeRDP/client/X11/xfreerdp /v:192.168.1.74 /u:administrator /p:Aa123! /app:\"||WINWORD\"");
    fprintf(stdout, "system ret:%d\n", status);
}

void *thread_func(void *arg) {
    fprintf(stdout, "pthread %lu start\n", pthread_self());
    system_call();
    fprintf(stdout, "pthread %lu end\n", pthread_self());
}

/**
 * @brief 在新进程中通过system启动另一个应用
 *
 * pthread_join 等待进程结束，释放资源
 * pthread_detach 非阻塞，可立即返回
 */
pthread_t start_thread_system() {
    pthread_t tid;
    int ret = 0;
    ret = pthread_create(&tid, NULL, thread_func, NULL);
#if 1    
    sleep(10);
#endif
    return tid;
}

/**
 * @brief fork后通过execl启动新应用
 */
void fork_execl_call() {
    if (fork()==0) {
        fprintf(stdout, "child %d start\n", getpid());
        //子进程
        execl("/home/zsy/Workspace/down/FreeRDP/client/X11/xfreerdp", "xfreerdp", "/v:192.168.1.74",
            "/u:administrator", "/p:Aa123!", "/app:||WINWORD", NULL);
        fprintf(stdout, "child %d end\n", getpid());
    } else {
        fprintf(stdout, "parent %d\n", getpid());
    }
}

int main() {
#if 0
    system_call();
#endif
#if 0
    pthread_t tid = start_thread_system();
    fprintf(stdout, "start_thread_system call end\n");
#if 0
    pthread_join(tid, NULL); //阻塞，等待进程结束
#endif
#endif
#if 1
    fork_execl_call();
    fprintf(stdout, "main end\n");
#endif
    return 0;
}

