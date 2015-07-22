/**
 * @brief 通过getenv函数获取环境变量值
 *
 * 修改环境变量
 * 1. 对所有用户生效，修改/etc/profile，添加
 *		export WLOG_APPENDER="FILE"
 * 2. 对当前会话生效，退出后就失效，在命令行中输入：
 *      $ export WLOG_APPENDER="FILE"
 * 取消环境变量
 *  $ unset WLOG_APPENDER
 * 查看环境变量
 * 1. 查看所有
 *  $ env
 * 2. 查看特定
 *  $ echo $WLOG_APPENDER
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *env = NULL;
	char *name = "WLOG_APPENDER";//"HOME";
	env = getenv(name);
	if (!env) {
		fprintf(stdout, "%s is not found\n", name);
		return 0;
	}
	fprintf(stdout, "%s env is %s\n", name, env);
	return 0;
}
