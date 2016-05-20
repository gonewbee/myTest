/**
  * @brief 查看在默认对齐模式下各个struct的大小
  * 
  * 自己的理解，默认字节对齐是使长度最大的成员在自己长度的整数倍，长度较小的成员可共同占用和长度最大成员相同大小。
  */
#include <stdio.h>

// sizeof值 3
typedef struct {
    char c1;
    char c2;
    char c3;
} struct_3char;

// sizeof值 2+2*1=4
typedef struct {
    short s1;
    char c1;
    char c2;
} struct_short_2char;

// sizeof值也是4
typedef struct {
    short s1;
    char c1;
} struct_short_char;

// 大小2+2*1+2 = 6
typedef struct {
    short s1;
    char c1;
    char c2;
    short s2;
} struct_short_2char_short;

// 根据字节对齐，大小为2+2+2+2 = 8
typedef struct {
    short s1;
    char c1;
    short s2;
    char c2;
} struct_short_char_short_char;

// 使长度最大的成员在自己长度的整数倍。
// 长度较小的成员可共同占用和长度最大成员相同大小
// 大小为8
typedef struct {
    int i1;
    char c1;
} struct_int_char;

// 大小还是为8
typedef struct {
    int i1;
    char c1;
    char c2;
} struct_int_2char;

// 大小依然为8
typedef struct {
    int i1;
    char c1;
    char c2;
    char c3;
} struct_int_3char;

// 大小为4+4+4 = 12
typedef struct {
    int i1;
    char c1;
    char c2;
    int i2;
} struct_int_2char_int;

// 大小为4+4+4+4 = 16
typedef struct {
    int i1;
    char c1;
    int i2;
    char c2;
} struct_int_char_int_char;

// 大小为4+4 = 8
typedef struct {
    int i1;
    short s1;
} struct_int_short;

int main(int argc, char *argv[]) {
    fprintf(stdout, "sizeof struct_3char:%lu\n", sizeof(struct_3char));
    fprintf(stdout, "sizeof struct_short_2char:%lu\n", sizeof(struct_short_2char));
    fprintf(stdout, "sizeof struct_short_char:%lu\n", sizeof(struct_short_char));
    fprintf(stdout, "sizeof struct_short_2char_short:%lu\n", sizeof(struct_short_2char_short));
    fprintf(stdout, "sizeof struct_short_char_short_char:%lu\n", sizeof(struct_short_char_short_char));

    fprintf(stdout, "\n");
    fprintf(stdout, "sizeof struct_int_char:%lu\n", sizeof(struct_int_char));
    fprintf(stdout, "sizeof struct_int_2char:%lu\n", sizeof(struct_int_2char));
    fprintf(stdout, "sizeof struct_int_3char:%lu\n", sizeof(struct_int_3char));
    fprintf(stdout, "sizeof struct_int_2char_int:%lu\n", sizeof(struct_int_2char_int));
    fprintf(stdout, "sizeof struct_int_char_int_char:%lu\n", sizeof(struct_int_char_int_char));

    fprintf(stdout, "\n");
    fprintf(stdout, "sizeof struct_int_short:%lu\n", sizeof(struct_int_short));

    // 使用od -t x1 foo12.tmp和od -t x1 foo16.tmp查看dump文件的不同
    struct_int_2char_int foo12 = {0x12345678, 'a', 'b', 0x87654321};
    struct_int_char_int_char foo16 = {0x12345678, 'a', 0x87654321, 'b'};
    FILE *fp = fopen("foo12.tmp", "wb");
    fwrite(&foo12, 1, sizeof(foo12), fp);
    fclose(fp);
    fp = fopen("foo16.tmp", "wb");
    fwrite(&foo16, 1, sizeof(foo16), fp);
    fclose(fp);
    return 0;
}

