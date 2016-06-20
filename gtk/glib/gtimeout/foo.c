#include <glib.h>

gboolean time_handler(gpointer data) {
	static int i = 0;
	int n = GPOINTER_TO_INT(data);
	i++;
	g_printf("%s: i:%d n:%d\n", __func__, i, n);
	return i < n;
}

gboolean stdin_data_available(GIOChannel *source, GIOCondition condition, 
	gpointer data) {
	gchar *msg;
	gsize len;
	g_io_channel_read_line(source, &msg, &len, NULL, NULL);
	g_printf("Input %u:%s\n", len, msg);
	g_free(msg);
	return TRUE;
}

int main(int argc, char *argv[]) {
	int n = 10;
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	GIOChannel *sin = g_io_channel_unix_new(0);

	g_timeout_add(1000, time_handler, GINT_TO_POINTER(n));
	g_io_add_watch(sin, G_IO_IN, stdin_data_available, NULL);

	g_main_loop_run(loop);
	return 0;
}

