/**
 * @brief 共享使用
 *
 * 应用启动时创建共享内存，退出时删除共享内存
 */

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include "ipcshm.h"

int main(int argc, char **argv) {
    int shmid=0;
    int ret = 0;
    char inStr[MAX_STRLEN];
    int inStrLen = 0;
    MY_BLOCK_T* block;
    /* 创建 */
    shmid = shmget(IPCSHMKEY, sizeof(MY_BLOCK_T), (IPC_CREAT|0666));
    /* 使用 */
    block = (MY_BLOCK_T*)shmat(shmid, (const void*)0, 0);
    block->size = 0;
    do {
        printf("please input string:");
        fgets(inStr, MAX_STRLEN, stdin);
        inStrLen = strlen(inStr);
        block->size = inStrLen;
        memcpy(block->str, inStr, inStrLen);
    }while(0!=strncmp(inStr, "quit", 4));
    /* 删除使用 */
    ret = shmdt((void *)block);
    /* 删除 */
    ret = shmctl(shmid, IPC_RMID, 0);
    return 0;
}

