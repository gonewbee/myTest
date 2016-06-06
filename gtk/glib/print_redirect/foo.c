/**
 * @brief glib的打印和打印重定向
 *
 * 编译: gcc foo.c `pkg-config --cflags --libs glib-2.0`
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

static FILE *g_file = NULL;

/**
 * @brief 打印函数，使用g_set_print_handler设置
 */
void log_file(const gchar *string) {
    if (!g_file) {
        g_file = fopen("out.txt", "wb");
        if (!g_file) {
            g_printerr("g_file is NULL!\n");
            exit(-1);
        }
    }
    fwrite(string, 1, strlen(string), g_file);
}

int main(int argc, char *argv[]) {
    g_print("Hello Word!\n");
    // 设置后g_print的打印会重定向log_file
    g_set_print_handler(log_file);
    g_print("Hello World set print!\n");
    return 0;
}

