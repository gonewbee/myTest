/**
 * @brief 测试fork函数，fork函数会产生新的进程。
 *
 * 通过ps aux命令可以看到fork后出现新的进程，pstree命令可以观察进程的父进程
 * getpid()可以获取当前进程pid，getppid()获取父进程pid
 */
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    fprintf(stdout, "enter main getpid:%d\n", getpid());
    sleep(5);
    pid = fork();
    if (0 == pid) {
        // 子进程中返回值为0
        fprintf(stdout, "child process getpid:%d pid:%d getppid:%d\n", getpid(), pid, getppid());
        // 父进程先退出并不影响子进程的继续执行
        sleep(3);
    } else if(pid > 0) {
        // 父进程中返回子进程pid
        fprintf(stdout, "parent process getpid:%d pid:%d\n", getpid(), pid);
    }
    sleep(3);
    fprintf(stdout, "process end!!getpid:%d pid:%d\n", getpid(), pid);
    return 0;
}
