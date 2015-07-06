/**
 * @breif 显示使用XGetImage从pixmap中保存的数据
 *
 * 编译动态库：gcc -o draw.so draw_tmp.c -lX11 -fPIC -shared
 * 编译测试文件：gcc draw_tmp.c -D_DEBUG -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_FUNC_ALL            (1L << 0)
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define PROP_MOTIF_WM_HINTS_ELEMENTS	5

typedef struct _app_context {
    Display *display;
    int screen_number;
    Screen *screen;
    XSetWindowAttributes winAttrib;
    int format;
    int depth;
    int scanline_pad;
    int offset;
    Atom _MOTIF_WM_HINTS;
}AppContext;

typedef struct _app_window {
    Window handle;
    Pixmap pixmap;
    int x;
    int y;
    int width;
    int height;
    GC gc;
}AppWindow;

struct _PropMotifWmHints
{
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long inputMode;
	unsigned long status;
};
typedef struct _PropMotifWmHints PropMotifWmHints;

/**
 * 设置完后，建立的窗口就没有Ubuntu自带的关闭、最小化和最大化这三个键了
 */
void setWindowDecorations(AppContext *cxt, Window window, int show)
{
	PropMotifWmHints hints;
	
	hints.decorations = (show) ? MWM_DECOR_ALL : 0;
	hints.functions = MWM_FUNC_ALL ;
	hints.flags = MWM_HINTS_DECORATIONS | MWM_HINTS_FUNCTIONS;
	hints.inputMode = 0;
	hints.status = 0;
	XChangeProperty(cxt->display, window, cxt->_MOTIF_WM_HINTS, cxt->_MOTIF_WM_HINTS, 32,
			PropModeReplace, (char*) &hints, PROP_MOTIF_WM_HINTS_ELEMENTS);
}

AppContext *app_context_new() {
    AppContext *cxt = (AppContext *)malloc(sizeof(AppContext));
    cxt->display = XOpenDisplay(NULL);
	cxt->_MOTIF_WM_HINTS = XInternAtom(cxt->display, "_MOTIF_WM_HINTS", False);
    cxt->screen_number = DefaultScreen(cxt->display);
    cxt->screen = ScreenOfDisplay(cxt->display, cxt->screen_number);
    cxt->format = ZPixmap;
    cxt->depth = 24;
    cxt->scanline_pad = 32;
    cxt->offset = 0;
}

void app_context_destroy(AppContext *cxt) {
    XCloseDisplay(cxt->display);
    free(cxt);
}

void init_app_window(AppContext *cxt, AppWindow *appWindow) {
    int input_mask;
    XGCValues gcv;
    XWMHints* InputModeHint;
    appWindow->handle = XCreateWindow(cxt->display, RootWindowOfScreen(cxt->screen),
						appWindow->x, appWindow->y,
						appWindow->width, appWindow->height,
						0,
						cxt->depth,
						InputOutput, DefaultVisual(cxt->display, 0),
						0, &cxt->winAttrib);    
    appWindow->pixmap = XCreatePixmap(cxt->display, appWindow->handle, 1366, 768, cxt->depth);
    memset(&gcv, 0, sizeof(gcv));
    appWindow->gc = XCreateGC (cxt->display, appWindow->handle, GCGraphicsExposures, &gcv);

    /* Set the input mode hint for the WM */
	InputModeHint = XAllocWMHints();
	InputModeHint->flags = (1L << 0);
	InputModeHint->input = True;

	XSetWMHints(cxt->display, appWindow->handle, InputModeHint);
	XFree(InputModeHint);
 
    input_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
				 ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
				 PointerMotionMask | Button1MotionMask | Button2MotionMask |
				 Button3MotionMask | Button4MotionMask | Button5MotionMask |
				 ButtonMotionMask | KeymapStateMask | ExposureMask |
				 VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask |
				 SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
				 ColormapChangeMask | OwnerGrabButtonMask;
	XSelectInput(cxt->display, appWindow->handle, input_mask); //设置要响应的事件
    setWindowDecorations(cxt, appWindow->handle, 0);	//设置后没有Ubuntu自带的关闭、最小和最大这三个键
}

void destory_app_window(AppContext *cxt, AppWindow *appWindow) {
    XFreePixmap(cxt->display, appWindow->pixmap);
    XFreeGC(cxt->display, appWindow->gc);
    XDestroyWindow(cxt->display, appWindow->handle);
}

/**
 * fileName:appWindow->x, appWindow->y, appWindow->width, appWindow->height, ax, ay, width, height, x, y, image->bytes_per_line,  (UINT32)tv.tv_sec, (UINT32)tv.tv_usec, (UINT32)appWindow->handle
 */
void draw_tmp_file(char *fileName) {
    AppContext *cxt = app_context_new();
    AppWindow appWindow;
    int ax, ay; //pixmap坐标
    int width, height; //copy图像的寬高
    int x, y; //window中图片的坐标
    int bytes_per_line;
    sscanf(fileName, "tmp/%u_%u_%u_%u_%u_%u_%u_%u_%u_%u_%u",
        &appWindow.x, &appWindow.y, &appWindow.width, &appWindow.height, &ax, &ay, &width, &height, &x, &y, &bytes_per_line);
    fprintf(stdout, "%d %d %d %d %d %d %d\n", ax, ay, width, height, x, y, bytes_per_line); //使用sscanf从格式化的字符串中读取整数
    init_app_window(cxt, &appWindow);
    XClearWindow(cxt->display, appWindow.handle);
    XMapWindow(cxt->display, appWindow.handle);
    XMoveWindow(cxt->display, appWindow.handle, appWindow.x, appWindow.y);
    XFlush(cxt->display);
    
    char *buf = (char *)malloc(width*height*4);
    FILE *fp = fopen(fileName, "r");
    fread(buf, 1, width*height*4, fp);
    printf("buf:%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
    XImage *image = XCreateImage (cxt->display,
			DefaultVisual(cxt->display, 0)/*CopyFromParent*/, cxt->depth,
			ZPixmap, 0,
			buf,
			width, height,
			cxt->scanline_pad, bytes_per_line
		);
    XPutImage(cxt->display, appWindow.pixmap, appWindow.gc,
			image, 0, 0, ax, ay, width, height);
    XFree(image);
    free(buf);
    fclose(fp);
    XCopyArea(cxt->display, appWindow.pixmap, appWindow.handle, appWindow.gc,
            ax, ay, width, height, x, y);

    XEvent event;
    while (1) {
        XNextEvent(cxt->display, &event); //获取事件
        if (event.type==ButtonPress) {
            fprintf(stdout, "ButtonPress\n");
            break;  
        }
    }
    
    destory_app_window(cxt, &appWindow);
    app_context_destroy(cxt);
}

#ifdef _DEBUG
#if 1
int main(int argc, char *argv[]) {
    if (argc!=2) {
        fprintf(stderr, "argc should be 2\n");
        return 1;
    }
    draw_tmp_file(argv[1]);
    return 0;
}
#else
int main(int argc, char *argv[]) {
    if (argc!=2) {
        fprintf(stderr, "argc should be 2\n");
        return 1;
    }
    Display *display = NULL;
    Window window;
    int screen_number;
    Screen *screen;
	XSetWindowAttributes winAttrib;
    Pixmap pixmap;
    GC gc;
    int input_mask;
    int format = ZPixmap;
    int depth = 24;
    int scanline_pad = 32;
    int offset = 0;
    int bytes_per_line = 0;
    int px = 0;
    int py = 0;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int win_width = 0;
    int win_height = 0;
    sscanf(argv[1], "tmp/%u_%u_%u_%u_%u_%u_%u",
        &px, &py, &width, &height, &x, &y, &bytes_per_line);
    fprintf(stdout, "%d %d %d %d %d %d %d\n", px, py, width, height, x, y, bytes_per_line); //使用sscanf从格式化的字符串中读取整数

    display = XOpenDisplay(NULL);
    screen_number = DefaultScreen(display);
    screen = ScreenOfDisplay(display, screen_number);
    window = XCreateWindow(display, RootWindowOfScreen(screen),
						100, 500,
						1025, 716,
						0,
						depth,
						InputOutput, DefaultVisual(display, 0),
						0, &winAttrib);
    input_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
				 ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
				 PointerMotionMask | Button1MotionMask | Button2MotionMask |
				 Button3MotionMask | Button4MotionMask | Button5MotionMask |
				 ButtonMotionMask | KeymapStateMask | ExposureMask |
				 VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask |
				 SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
				 ColormapChangeMask | OwnerGrabButtonMask;
	XSelectInput(display, window, input_mask); //设置要响应的事件
    XMapWindow(display, window);
    XFlush(display);
    pixmap = XCreatePixmap(display, window, 1366, 768, depth);
    gc = XCreateGC (display, window, 0, NULL); 

    char *buf = (char *)malloc(width*height*4);
    FILE *fp = fopen(argv[1], "r");
    fread(buf, 1, width*height*4, fp);
    XImage *image = XCreateImage (display,
			CopyFromParent, depth,
			ZPixmap, 0,
			buf,
			width, height,
			scanline_pad, bytes_per_line
		);
    XPutImage(display, pixmap, gc,
			image, 0, 0, px, py, width, height);
    XFree(image);
    free(buf);
    fclose(fp);
    XCopyArea(display, pixmap, window, gc,
            px, py, width, height, x, y);
    XFlush(display);
    getchar();
    
    return 0;
}
#endif
#endif

