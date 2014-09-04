/**
 * 使用共享内存
 */

#include <stdio.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include "ipcshm.h"

int main(int argc, char *argv) {
    int shmid=-1;
    char outStr[MAX_STRLEN];
    int outStrLen = 0;
    int ret = 0;
    MY_BLOCK_T* block;
    shmid = shmget(IPCSHMKEY, 0, 0);
    if (-1==shmid) {
        printf("shmget error:%s\n", strerror(errno));
        return 1;
    }
    block = (MY_BLOCK_T*)shmat(shmid, (const void*)0, 0);
    outStrLen = block->size;
    memcpy(outStr, block->str, outStrLen);
    printf("len:%d;;str:%s\n", outStrLen, outStr);
    ret = shmdt((void*)block);
//    ret = shmctl(shmid, IPC_RMID, 0);
    return 0;
}

