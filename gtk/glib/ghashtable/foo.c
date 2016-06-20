/**
 * @brief 测试glib哈希表
 *
 * 编译: gcc foo.c `pkg-config --cflags --libs glib-2.0`
 */
#include <glib.h>

void zs_hash_table_func(gpointer key, gpointer value, gpointer user_data) {
	g_printf("%s key[%s] value[%s]\n", user_data, key, value);		
}

gboolean zs_hash_table_finder(gpointer key, gpointer value, gpointer user_data) {
	GString *k = g_string_new(key);
	// 当key长度大于3时返回TRUE
	if (k->len > 3) {
		return TRUE;
	}
	return FALSE;
}

int main(int argc, char *argv[]) {
	GHashTable *hash = g_hash_table_new(g_str_hash, g_str_equal);
	// 插入
	g_hash_table_insert(hash, "one", "aaaa");
	g_hash_table_insert(hash, "two", "bbbb");
	g_hash_table_insert(hash, "three", "cccc");
	g_printf("There are %d keys in the hash\n", g_hash_table_size(hash));

	char *key = "one";
	// 查找
	g_printf("The value of [%s] is %s\n", key, g_hash_table_lookup(hash, key));
	char *key2 = "four";
	g_printf("The value of [%s] is %s\n", key2, g_hash_table_lookup(hash, key2));

	// 删除
	gboolean found = g_hash_table_remove(hash, key);
	g_printf("The key [%s] was %sfound and removed\n", key, found ? "" : "not ");

	// 遍历
	g_hash_table_foreach(hash, zs_hash_table_func, "g_hash_table_foreach");

	gpointer v = g_hash_table_find(hash, zs_hash_table_finder, NULL);
	g_printf("v is %s\n", v);

	g_hash_table_destroy(hash);
	return 0;
}

