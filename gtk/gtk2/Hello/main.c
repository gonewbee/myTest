/**
 * @brief 显示gtk窗口
 * 
 * 编译: gcc main.c `pkg-config --libs --cflags gtk+-2.0`
 */
#include <gtk/gtk.h>

/**
 * @brief 回调函数，本例忽略data参数
 */
static void hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World!\n");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    /* 如果在delete-event信号处理中返回FALSE，GTK会发出destroy信号。
     * 返回TRUE意味着不想窗口被销毁，这有助于发出“确认退出” 这类对话框*/
    g_print("delete event occurred\n");

    /* 将TRUE改为FALSE，主窗口会被销毁*/
    return TRUE;
}

/**
 * @brief 另一个回调
 */
static void destroy(GtkWidget *widget, gpointer data) {
    g_print("destroy enter!\n");
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    /* GtkWidget是widgets的存储类型 */
    GtkWidget *window;
    GtkWidget *button;

    /* 所有的GTK程序都要调用这句 */
    gtk_init(&argc, &argv);
    
    /* 创建新窗口 */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* 当窗口管理器（通常是关闭操作）向窗口发送"delete-event"信号时，
     * 我们要求调用前面定义的delete_event() 函数。向这个回调函数传递的数据为NULL*/
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
    
    /* 将"destroy"事件连到一个信号处理，当在window上调用gtk_widget_destroy()或
     * 在"delete-event"回调中返回FALSE时触发这个事件*/
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    /* 设置窗口的边框宽度 */
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    /* 创建一个标签为"Hello World"的按键 */
    button = gtk_button_new_with_label("Hello world");

    /* 当按键接收到"clicked"信号时，调用hello()函数，参数为NULL */
    g_signal_connect(button, "clicked", G_CALLBACK(hello), NULL);

    /* 当点击时通过调用gtk_widget_destroy(window)销毁窗口，
     * 销毁信号可以从这或窗口管理器产生。*/
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    
    /* 将button包到window */
    gtk_container_add(GTK_CONTAINER(window), button);

    /* 最后一步，显示最新创建的widget */
    gtk_widget_show(button);

    /* 和窗口 */
    gtk_widget_show(window);

    /* 所有的GTK程序都有gtk_main()，控制停止并等待事件产生。 */
    gtk_main();
    return 0;
}

