/**
 * @breif 测试static变量的作用域
 */
#include <stdio.h>

static int a=5;

static void my_static_print(int x) {
    printf("%s after add a:%d\n", __func__, x+a);
}

void my_print(int x) {
    static int b = 5;
    printf("%s after add b:%d\n", __func__, x+b);
    b++;
}

#if 0
/**
 * 变量a是static类型的全局变量，在本文件中可以访问到
 * 而b是static类型的局部变量，虽然其值可以保存但不能被其它函数访问
 */
voit test_var() {
    printf("var a:%d\n", a);
    printf("var b:%d\n", b);
}
#endif

#ifdef _DEBUG_
int main() {
    int x = 5;
    int y = 10;
    char *cpr;
#if 0    
    int *ipr=NULL;
#else
    int *ipr=&y;
#endif
    double *dpr;
    for (;x>=0;x--) {
        my_static_print(x);
        my_print(x);
    }
    printf("sizeof cpr:%d *cpr:%d char *:%d char:%d\n", sizeof(cpr), sizeof(*cpr), sizeof(char *),sizeof(char));
    printf("sizeof ipr:%d *ipr:%d int *:%d int:%d\n", sizeof(ipr), sizeof(*ipr), sizeof(int *), sizeof(int));
    printf("sizeof dpr:%d *dpr:%d double *:%d double:%d\n", sizeof(dpr), sizeof(*dpr), sizeof(double *), sizeof(double));
    printf("x:%d &x:0x%x y:%d &y:%x\n", x, &x, y, &y);
    if (ipr)
        printf("value ipr:0x%x *ipr:%d\n", ipr, *ipr);
    else    
        printf("value ipr:0x%x\n", ipr);
    ipr = &x;
    printf("value ipr:0x%x *ipr:%d\n", ipr, *ipr);
    *ipr = x;
    printf("value ipr:0x%x *ipr:%d\n", ipr, *ipr);
    return 0;
}
#endif

