/**
 * 发送指定pid信号
 */

#include <stdio.h>
#include <signal.h>

int main(int argc, char **argv) {
    char str[15];
    pid_t pid = 0;
    printf("please input the pid to send SIGTERM\n");
    fgets(str, 15, stdin);
    pid = atoi(str);
    printf("send SIGTERM to %u\n", pid);
    /* 向指定pid发送SIGTERM */
    kill(pid, SIGTERM);
    return 0;
}

