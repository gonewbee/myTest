#include <glib.h>

gboolean gio_in(GIOChannel *gio, GIOCondition condition, gpointer data) {	
	GIOStatus ret;
	GError *err = NULL;
	gchar *msg;
	gsize len;

	ret = g_io_channel_read_line(gio, &msg, &len, NULL, &err);
	if (G_IO_STATUS_ERROR == ret) {
		g_printf("Error:%s\n", err->message);
	}
	g_printf("Read %d: %s\n", len, msg);
	g_free(msg);
	return TRUE;
}

gboolean gio_out(GIOChannel *gio, GIOCondition condition, gpointer data) {		
	const gchar *msg = "Hello Word!!\n";
	GIOStatus ret;
	GError *err = NULL;
	gsize len;

	ret = g_io_channel_write_chars(gio, msg, -1, &len, &err);
	if (G_IO_STATUS_ERROR == ret) {
		g_error("Error:%s\n", err->message);
	}
	g_printf("Wrote:%d\n", len);
	return TRUE;
}

void init_channels(void) {
	GIOChannel *gio_read, *gio_write;
	gio_read = g_io_channel_unix_new(0);
	gio_write = g_io_channel_unix_new(1);

	g_io_add_watch(gio_read, G_IO_IN, gio_in, NULL);
	// g_io_add_watch(gio_write, G_IO_OUT, gio_out, NULL);
}

int main(int argc, char *argv[]) {
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	init_channels();
	g_main_loop_run(loop);
	return 0;
}

