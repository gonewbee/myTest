#include <stdio.h>
#include "bar.h"

extern int global_a;
// static类型的全局变量不能extern，编译成功，链接失败
// extern int global_b;

int main() {
    int a = 0;
    print_global();
    global_a = 22;
    print_global();
//    global_b = 55;
    a = get_static_value();
    fprintf(stdout, "get_static_value:%d\n", a);
    return 0;
}

