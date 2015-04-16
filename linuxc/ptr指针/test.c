/**
 * @brief 测试函数指针参数问题
 */
#include <stdio.h>
#include <pthread.h>

typedef struct {
    unsigned char ch;
    unsigned int nums[2];
    unsigned int id;
}_test_info_t;

static void print_info(const char *str, _test_info_t info) {
    printf("info:");
    printf("%c; %d %d; %d;", info.ch, info.nums[0], info.nums[1], info.id);
    printf("%s\n", str);
}

static void *thread_test_func2(void *arg) {
    _test_info_t *info;
    printf("%s arg:%p\n", __func__, arg);
    info = arg;
    printf("%s info:%p\n", __func__, info);
    print_info(__func__, *info);
    /* 修改会影响test里的info内容 */
    (*info).ch = 'b';
}

static void *thread_test_func(void *arg) {
    _test_info_t info;
    printf("%s arg:%p\n", __func__, arg);
    info = *(_test_info_t *)arg;
    printf("%s info:%p\n", __func__, &info);
    print_info(__func__, info);
    /* 不会影响test里info的内容 */
    info.ch = 'b';
}

static void test() {
    pthread_t pid;
    _test_info_t info;
    info.ch = 'a';
    info.nums[0] = 1234;
    info.nums[1] = 5678;
    info.id = 10001;
    printf("%s info:%p\n", __func__, &info);
    pthread_create(&pid, NULL, thread_test_func, (void *)(&info));
    sleep(1);
    print_info(__func__, info);
    pthread_join(pid, NULL);
    printf("%s end\n", __func__);
}

int main(int argc, char **argv) {
    test();
    return 0;
}

