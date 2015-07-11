/**
 * @brief 使用getpwuid获取用户根目录
 *
 * 参考：http://stackoverflow.com/questions/2910377/get-home-directory-in-linux-c
 * Note: if you need this in a threaded application, you'll want to use getpwuid_r instead.
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define MAXPATHLEN 64

int main(int argc, char *argv[]) {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    fprintf(stdout, "当前用户根目录：%s\n", homedir);
    char current_path[MAXPATHLEN] = {0};
    if (getcwd(current_path, MAXPATHLEN)) {
        //使用getcwd获取当前工作路径
        fprintf(stdout, "当前工作路径：%s\n", current_path);
    }
    return 0;
}

