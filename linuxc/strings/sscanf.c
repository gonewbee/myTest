/**
 * @brief c使用sscanf提取两个字符之间的字符串
 */
#include <stdio.h>

static void sscanf_test(char *cmd) {
	char program[256] = {0};
	char filename[256] = {0};
	if (0==strncmp(cmd, "startApp:[", 10)) {
		sscanf(cmd, "%*[^[][%[^]]", program); //取[和]之间的字符
		fprintf(stdout, "program:%s\n",  program);
	} else if (0==strncmp(cmd, "startDoc:[", 10)) {
		sscanf(cmd, "%*[^[][%[^#]", program); //取[和#之间的字符
		fprintf(stdout, "program:%s\n",  program);
		sscanf(cmd, "%*[^#]#%[^]]", filename); //取#和]之间的字符
		fprintf(stdout, "filename:%s\n",  filename);
	}
}

int main() {
	char *str1 = "startApp:[||WINWORD]";
	char *str2 = "startDoc:[c:\\test.exe#\\\\tsclient\\home\\test.doc]";
	sscanf_test(str1);
	sscanf_test(str2);
}

