#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

static FILE *g_log_file = NULL;

void log_to_file(const gchar *log_domain,
        GLogLevelFlags log_level,
        const gchar *message,
        gpointer user_data) {
    if (!g_log_file) {
        g_log_file = fopen("log.txt", "wb");
        if (!g_log_file) {
            exit(-1);
        }
    }
    fprintf(g_log_file, "%s", message);
}

int main(int argc, char *argv[]) {
#if 0
    g_log_set_handler(NULL, G_LOG_LEVEL_DEBUG|G_LOG_LEVEL_WARNING,
            log_to_file, NULL);
#else
    g_log_set_handler(NULL, G_LOG_LEVEL_MASK,
            log_to_file, NULL);
#endif
    g_debug("This is a debug msg\n");
    g_warning("This is warning\n");
    g_message("This is message\n");
    return 0;
}

