/**
 * 快速排序
 * http://blog.csdn.net/morewindows/article/details/6684558
 * http://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_buffer(int *list, int len) {
    int i=0;
    for (i=0; i<len; i++) {
        if (i%16==0)
            printf("\n");
        printf("%4d ", list[i]);
    }
    printf("\n");
}

/**
 * wiki上代码
 */
void quick_sort (int data[], size_t left, size_t right) {
	size_t p = (left + right) / 2;  //取中间的数作为比较值
	int pivot = data[p];
	size_t i = left,j = right;
	for ( ; i < j;) {
		while (! (i>= p || pivot < data[i]))
			++i;
		if (i < p) {
			data[p] = data[i];
			p = i;
		}
		while (! (j <= p || data[j] < pivot))
			--j;
		if (j > p) {
			data[p] = data[j];
			p = j;
		}
	}
	data[p] = pivot;
	if (p - left > 1)
		quick_sort(data, left, p - 1);
	if (right - p > 1)
		quick_sort(data, p + 1, right);
}

void quick_sort_2(int *list, int start, int end) {
    int X = list[end];  //取最后一个元素做比较值
    int tmp;
    int midIndex=start, index=start;
    if (start>=end)
        return;
    while (index<end) {
        if (list[index]<X) {
            if (index!=midIndex) {
                tmp = list[midIndex];
                list[midIndex] = list[index];
                list[index] = tmp;
            }
            midIndex++;
        }
        index++;
    }
    tmp = list[midIndex];
    list[midIndex] = list[end];
    list[end] = tmp;
    quick_sort(list, start, midIndex-1);
    quick_sort(list, midIndex+1, end);
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
    quick_sort(list, 0, len-1);
    end = clock();
    printf("sort time: %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
#ifdef _PRINTBUF
    printf("after sort:");
    print_buffer(list, len);
#endif
    free(list);
    return 0;
}
