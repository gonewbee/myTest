/**
 * @brief glib的GList测试
 *
 * 编译: gcc foo.c `pkg-config --cflags --libs glib-2.0`
 */
#include <glib.h>

/**
 * @brief g_list_foreach的回调函数
 */
void zs_list_func(gpointer data, gpointer user_data) {
	g_printf("[%s] data:%s\n", user_data, data);
}

int main(int argc, char *argv[]) {
	GList *list = NULL;
	// 在结尾添加
	list = g_list_append(list, "ONE");
	list = g_list_append(list, "TWO");
	list = g_list_append(list, "THREE");
	// 获取list长度
	g_printf("list length:%d\n", g_list_length(list));
	// 使用g_list_foreach遍历list
	g_list_foreach(list, zs_list_func, "g_list_foreach");

	// 在list头部添加
	list = g_list_prepend(list, "ZERO");
	// 获取list长度
	g_printf("list length:%d\n", g_list_length(list));
	// 使用g_list_foreach遍历list
	g_list_foreach(list, zs_list_func, "g_list_foreach");

	char *data = "TWO";
	int index = 0;
	// 查找数据在列表中的位置
	g_printf("[%s] index is %d\n", data, g_list_index(list, data));
	// 获取列表指定位置的数据
	g_printf("position [%d] data is %s\n", index, g_list_nth_data(list, index));

	// 遍历列表
	GList *it = g_list_first(list);
	for (;it;it=it->next) {
		g_printf("next is [%s]\n", it->data);		
	}

	g_list_free(list);
	return 0;
}

