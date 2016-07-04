/**
 * 在c语言中内联汇编，使用AT&T风格
 * 编译: gcc -nostdlib foo.c
 */

/**
 * @brief 通过系统调用打印字符串
 */
void my_printf(char *s, int len) {
    long ret;
    __asm__ volatile(
        "int $0x80"     /* 系统调用 */
        : "=a" (ret)    /* 返回值eax("a") */
        : "0"(4),       /* 系统调用号sys_write */
          "b"(1),       /* 参数ebx，文件句柄1=标准输出 */
          "c"(s),       /* 参数ecx, 字符串 */
          "d"(len)      /* 参数edx, 字符长度 */
    );
}

/**
 * @brief 函数入口
 */
void _start() {
    /* main body of program: call main(), etc */
    char *s = "Hello world!\n";
    my_printf(s, 13);

#if 1
    /* exit system call */
    asm("movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );
#endif
}
