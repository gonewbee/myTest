/**
 * @brief 结构体作为参数传递
 *
 * 结构体作为参数时，在函数中修改不改变传递的结构体值，
 * 和普通参数一样，要想同时改变的话要传指针。
 */
#include <stdio.h>

typedef struct _MyStruct {
    char ch;
    int no;
    int value;
}MyStruct;

void print_MyStruct(MyStruct myStruct) {
    printf("%d %d %c\n", myStruct.no, myStruct.value, myStruct.ch);
}

void change_MyStruct(MyStruct myStruct) {
    printf("%s myStruct:%u\n", __func__, &myStruct);
    myStruct.ch += 1;
    myStruct.no +=1;
    myStruct.value*=10;
}

void changeMyStruct(MyStruct *myStruct) {
    printf("%s myStruct:%u\n", __func__, myStruct);
    myStruct->ch += 1;
    myStruct->no +=1;
    myStruct->value*=10;
}

int main(int argc, char **argv) {
    MyStruct myStruct;
    printf("%s myStruct:%u\n", __func__, &myStruct);
    print_MyStruct(myStruct);
    myStruct.ch = 'a';
    myStruct.no = 0;
    myStruct.value = 1;
    print_MyStruct(myStruct);
    printf("after call\n");
    change_MyStruct(myStruct);
    print_MyStruct(myStruct);
    printf("after ptr call\n");
    changeMyStruct(&myStruct);
    print_MyStruct(myStruct);
    return 0;
}

