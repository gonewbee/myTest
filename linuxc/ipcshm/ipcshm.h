#ifndef _IPCSHM_H_
#define _IPCSHM_H_

#define IPCSHMKEY 0x12138
#define MAX_STRLEN 100
typedef struct {
    int size;
    char str[MAX_STRLEN];
}MY_BLOCK_T;

#endif

