/**
 * 打印出数组的address，在子线程中打印该地址对应的数值
 * 使用pthread_cond_t 做同步
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uchar unsigned char

static pthread_mutex_t mutex;
static pthread_cond_t cond;

void printBuf(uchar *buf, int len) {
    int i = 0;
    printf("address:%d\n", buf);
    for (i=0; i<len; i++) {
        printf("%x ", buf[i]);
    }
    printf("\n");
}

void *test_thread(void *arg) {
    uchar *inaddr;
    char instr[15];
    int inNum = 0;
    printf("please input:\n");
    fgets(instr, 15, stdin);
    inNum = atoi(instr);
    inaddr = (uchar *)inNum;
    printf("in thread:%x\n", *inaddr); 
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    
    printf("please input:\n");
    fgets(instr, 15, stdin);
    inNum = atoi(instr);
    inaddr = (uchar *)inNum;
    printf("in thread:%x\n", *inaddr); 
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv) {
    uchar *buf=NULL;
    pthread_t thread_id;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&thread_id, NULL, test_thread, NULL);
    buf = (uchar *)malloc(20);
    printBuf(buf, 20);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    memset(buf, 0xaa, 20);
    printBuf(buf, 20);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    free(buf);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
