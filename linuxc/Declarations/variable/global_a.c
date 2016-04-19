#include <stdio.h>

// 未初始化的全局变量在.bss段，通过objdump -x XX可查看到
// 编译出文件只有9k，由此可见，bss类型全局变量只占运行时内存空间
// 不占文件空间
// **在大多数操作系统，在加载程序时会把bss全局变量全部清零。
int global_a[10240];

int main(int argc, char *argv[]) {
    // 全局变量自动清零，打印0 0 0 0
    fprintf(stdout, "%x %x %x %x\n", global_a[0], global_a[1], global_a[2], global_a[3]);
    return 0;
}

