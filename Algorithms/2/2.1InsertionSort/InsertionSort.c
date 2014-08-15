/**
 * 插入排序
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/**
 * @param *list 要排序的数组
 * @param len 数组中的元素数量
 */
void insertion_sort(int *list, int len) {
    int i, j;
    int tmp;
    for (i=1; i<len; i++) {
        tmp = list[i];
        j = i-1;
        while ((j>=0) && (list[j]>tmp)) {
            list[j+1] = list[j];
            j--;
        }
        list[j+1] = tmp;
    }
}

/**
 * @brief 随机产生数组，随机数大小[0:10000]
 * @param *list 输出，产生的随机数存储
 * @param len 要产生的随机数的个数
 */
void createList(int *list, int len) {
    int i, j;
    srand((int)time(0));
    for (i=0; i<len; i++) {
        j=1+(int)(10000.0*rand()/(RAND_MAX+1.0));
        list[i] = j;
    }
}

void print_buffer(int *list, int len) {
    int i=0;
    for (i=0; i<len; i++) {
        if (i%16==0)
            printf("\n");
        printf("%4d ", list[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int len = 16;
    clock_t start, end;
    if (argc>1) {
        len = atoi(argv[1]);
    }
    printf("data len: %d\n", len);
    int *list = (int *)malloc(len*sizeof(int));
    createList(list, len);
//    printf("before sort:");
//    print_buffer(list, len);
    start = clock();
    insertion_sort(list, len);
    end = clock();
    printf("sort time: %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
//    printf("after sort:");
//    print_buffer(list, len);
    free(list);
    return 0;
}
