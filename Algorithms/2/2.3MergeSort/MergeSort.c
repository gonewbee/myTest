/**
 * 归并排序
 * http://zh.wikipedia.org/wiki/%E5%BD%92%E5%B9%B6%E6%8E%92%E5%BA%8F
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/**
 * @breif 归并两个有序数组
 */
void merge_array(int *list1, int list1_size, int *list2, int list2_size) {
    int i=0, j=0, k=0;

    //声明临时数组用于存储归并结果
    int *tmp = malloc((list1_size+list2_size)*sizeof(int));
    //只要有一个数组到达尾部就跳出循环
    while ((i<list1_size) && (j<list2_size)) {
        //将较小的数放到临时数组中，同时移动指针
        tmp[k++] = list1[i]<list2[j] ? list1[i++] : list2[j++];
    }
    //如果 list1 还有元素，把剩下的数据直接放到结果数组
    while (i<list1_size)
        tmp[k++] = list1[i++];
    //如果 list2 还有元素，把剩下的数据直接放到结果数组
    while (j<list2_size)
        tmp[k++] = list2[j++];

    //把结果copy到list1中
    memcpy(list1, tmp, k*sizeof(int));
    free(tmp);
}

/**
 * @param *list 要排序的数组
 * @param len 数组中的元素数量
 */
void merge_sort(int *list, int len) {
    if (len>1) {
        //将数组平均分成两个部分
        int *list1 = list;
        int list1_size = len/2;
        int *list2 = list+list1_size;
        int list2_size = len-list1_size;
        //分别归并排序
        merge_sort(list1, list1_size);
        merge_sort(list2, list2_size);

        //归并
        merge_array(list1, list1_size, list2, list2_size);
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
#ifdef _PRINTBUF
    printf("before sort:");
    print_buffer(list, len);
#endif
    start = clock();
    merge_sort(list, len);
    end = clock();
    printf("sort time: %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
#ifdef _PRINTBUF
    printf("after sort:");
    print_buffer(list, len);
#endif
    free(list);
    return 0;
}
