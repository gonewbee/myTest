/**
 * @brief 共享使用
 *
 * 应用启动时创建共享内存，退出时删除共享内存
 */

#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include "ipcshm.h"

static int shmid = -1;
static int semid = -1;
static MY_BLOCK_T* block=NULL;
static struct sembuf sp_wait = {0, -1, SEM_UNDO};
static struct sembuf sp_wakeup = {0, 1, SEM_UNDO};

int main(int argc, char **argv) {
    int ret = 0;
    char inStr[MAX_STRLEN];
    int inStrLen = 0;
    /* 创建 */
    shmid = shmget(IPCSHMKEY, sizeof(MY_BLOCK_T), (IPC_CREAT|0666));
    if (-1==shmid) {
        printf("shmget error:%s\n", strerror(errno));
        return 1;
    }
    semid = semget(IPCSEMKEY, 1, (IPC_CREAT|IPC_EXCL|0666));
    if (-1==semid) {
        printf("semget error:%s\n", strerror(errno));
        goto endf;
    }
    /* 初始化信号量 */  
    if (-1==semctl(semid, 0, SETVAL, 1)) {
        perror("semctl");
        goto endf;
    }
    /* 使用 */
    block = (MY_BLOCK_T*)shmat(shmid, (const void*)0, 0);
    block->size = 0;
    do {
        if (-1==semop(semid, &sp_wait, 1)) {
            perror("semop wait");
        }
        printf("please input string:");
        fgets(inStr, MAX_STRLEN, stdin);
        inStrLen = strlen(inStr);
        block->size = inStrLen;
        memcpy(block->str, inStr, inStrLen);
        if (-1==semop(semid, &sp_wakeup, 1)) {
            perror("semop wakeup");
        }
    } while(0!=strncmp(inStr, "quit", 4));
    /* 删除使用 */
    ret = shmdt((void *)block);
endf:
    /* 删除 */
    ret = semctl(semid, 0, IPC_RMID);
    ret = shmctl(shmid, IPC_RMID, 0);
    return 0;
}

