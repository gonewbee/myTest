/**
 * 使用共享内存，使用信号量进行进程间同步
 */

#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include "ipcshm.h"

static int shmid = -1;
static int semid = -1;
static MY_BLOCK_T* block;
static struct sembuf sp_wait = {0, -1, SEM_UNDO};
static struct sembuf sp_wakeup = {0, 1, SEM_UNDO};

int main(int argc, char *argv) {
    char outStr[MAX_STRLEN];
    int outStrLen = 0;
    int ret = 0;
    shmid = shmget(IPCSHMKEY, 0, 0);
    if (-1==shmid) {
        printf("shmget error:%s\n", strerror(errno));
        return 1;
    }
    semid = semget(IPCSEMKEY, 0, 0);
    if (-1==semid) {
        perror("semget");
        return 1;
    }
    block = (MY_BLOCK_T*)shmat(shmid, (const void*)0, 0);
    do {
        /* 等待信号量 */
        semop(semid, &sp_wait, 1);
        outStrLen = block->size;
        memcpy(outStr, block->str, outStrLen);
        if (outStrLen<MAX_STRLEN)
            outStr[outStrLen] = '\0';
        printf("len:%d;;str:%s\n", outStrLen, outStr);
        semop(semid, &sp_wakeup, 1);
    } while(0!=strncmp(outStr, "quit", 4));
    ret = shmdt((void*)block);
//    ret = shmctl(shmid, IPC_RMID, 0);
    return 0;
}

