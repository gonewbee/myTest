/**
 * 测试环形buffer的使用
 *
 * 1. 编译时加-lpthread
 * 2. 测试时输入1-9之间的数字，从buf中读出相应个数的数，输入‘q’推出。
 * @file ringBuffer.c
 * @author zhangshy
 * @date 2014-08-01
 */

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

/**
 * 环形buf长度BUFFER_SIZE+1
 *
 * 若使用pbuf->wptr &= (BUFFER_SIZE<<1)|0x1的方式将索引限定在[0:2*BUFFER_SIZE+1]，
 * BUFFER_SIZE的取值的格式为连续的二进制的N个1
 */
#define BUFFER_SIZE 0xF 
#define de_printf(X,...) printf(X, ##__VA_ARGS__)

typedef struct {
    uint32_t wptr;   ///< buffer存储位置，索引范围：[0:2*BUFFER_SIZE+1]
    uint32_t rptr;   ///< buffer读取位置，索引范围：[0:2*BUFFER_SIZE+1]
    uint8_t buffer[BUFFER_SIZE+1];  ///< buffer大小为BUFFER_SIZE+1
}ring_buffer_t;

/** 测试用环形buffer */
static ring_buffer_t tbuf;
/** 允许线程执行标志位 */
static uint32_t running=0;
/** 互斥锁 */
static pthread_mutex_t bLock = PTHREAD_MUTEX_INITIALIZER;

__inline uint32_t ring_buffer_is_empty(ring_buffer_t *pbuf) {
    return (pbuf->wptr==pbuf->rptr);
}

/**
 * @brief buffer是否已满
 *
 * 当wptr和rptr之间的差值为BUFFER_SIZE+1时，buffer为满;
 */
__inline uint32_t ring_buffer_is_full(ring_buffer_t *pbuf) {
    return ((pbuf->wptr^pbuf->rptr)==(BUFFER_SIZE+1));
}

__inline void ring_buffer_flush(ring_buffer_t *pbuf) {
    pbuf->wptr = pbuf->rptr = 0;
}

/**
 * @brief 向环形buf中存数
 *
 * wptr的索引：[0:2*BUFFER_SIZE+1]，若BUFFER_SIZE=0xF，则wptr：[0:31]
 */
void fill_ring_buffer(ring_buffer_t *pbuf, uint8_t data) {
    pbuf->buffer[pbuf->wptr&BUFFER_SIZE] = data;
    pbuf->wptr++;
    pbuf->wptr &= (BUFFER_SIZE<<1)|0x1;
}

uint8_t dump_ring_buffer(ring_buffer_t *pbuf) {
    uint8_t data;
    data = pbuf->buffer[pbuf->rptr&BUFFER_SIZE];
    pbuf->rptr++;
    pbuf->rptr &= (BUFFER_SIZE<<1)|0x1;

    return data;
}

/**
 * 向测试buf中写入数据的线程
 */
void *write_ring_buffer_thread(void *arg) {
    int i=0;
    int *sleep_time=(int *)arg;
    de_printf("%s begin sleep:%d\n", __func__, *sleep_time);
    while(running) {
        de_printf("-------%s w:%03d r:%03d i:%d\n", __func__, tbuf.wptr, tbuf.rptr, i);
        pthread_mutex_lock(&bLock);
        if (ring_buffer_is_full(&tbuf))
            de_printf("-------buffer is full\n");
        else {
            fill_ring_buffer(&tbuf, (uint8_t)i);
            i++;
        }
        pthread_mutex_unlock(&bLock);
        usleep(*sleep_time);
    }
    de_printf("%s end\n", __func__);
}

int main(int argc, char **argv) {
    pthread_t thread_id;
    char c;
    int num;
    int i=0;
    uint8_t data;
    int sleep_time=500000;
    ring_buffer_flush(&tbuf);   //先清空buffer标志位
    running=1;
    pthread_create(&thread_id, NULL, write_ring_buffer_thread, (void *)&sleep_time);
    while((c=getchar())!='q') {
        if (c<'0' || c>'9')
            continue;
        num=c-'0';
        de_printf("=====try to read:%d\n", num);
        pthread_mutex_lock(&bLock);
        for (i=0; i<num; i++) {
            if (ring_buffer_is_empty(&tbuf)) {
                de_printf("=====buf is empty\n");
            } else {
                data = dump_ring_buffer(&tbuf);
                de_printf("=====w:%03d r:%03d data:%d\n", tbuf.wptr, tbuf.rptr, data);
            }
        }
        pthread_mutex_unlock(&bLock);
    }
    running=0;
    pthread_join(thread_id, NULL);
}
