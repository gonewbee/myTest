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
    Visual *visual;
    Pixmap pixmap;
}AppContext;

typedef struct _app_window {
    Window handle;
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

void get_pixmap_info(AppContext *cxt) {
	int i;
	int vi_count;
	XVisualInfo* vi;
	XVisualInfo* vis;
	XVisualInfo template;
	XWindowAttributes window_attributes;

    memset(&template, 0, sizeof(template));
    template.class = TrueColor;
	template.screen = cxt->screen_number;
    if (XGetWindowAttributes(cxt->display, RootWindowOfScreen(cxt->screen), &window_attributes) == 0)
	{
        fprintf(stderr, "XGetWindowAttributes error!!\n");
		return;
	}
    vis = XGetVisualInfo(cxt->display, VisualClassMask | VisualScreenMask, &template, &vi_count);

	if (!vis)
	{
		fprintf(stderr, "XGetVisualInfo failed");
		return;
	}
    for (i = 0; i < vi_count; i++)
	{
		vi = vis + i;
		if (vi->visual == window_attributes.visual)
		{
            fprintf(stdout, "get_pixmap_info get visual\n");
			cxt->visual = vi->visual;
			break;
		}
	}
    XFree(vis);
    if (cxt->visual == NULL) {
        fprintf(stderr, "get_pixmap_info visul null\n");
    }
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
    cxt->visual = NULL/*DefaultVisual(cxt->display, cxt->screen_number)*/;
}

void app_context_destroy(AppContext *cxt) {
    XFreePixmap(cxt->display, cxt->pixmap);
    XCloseDisplay(cxt->display);
    free(cxt);
}

void init_app_window(AppContext *cxt, AppWindow *appWindow) {
    int input_mask;
    XGCValues gcv;
    XWMHints* InputModeHint;
#if 1
    appWindow->handle = XCreateWindow(cxt->display, RootWindowOfScreen(cxt->screen),
						appWindow->x, appWindow->y,
						appWindow->width, appWindow->height,
						0,
						cxt->depth,
						InputOutput, cxt->visual,
						0, &cxt->winAttrib);    
#else
    appWindow->handle = XCreateSimpleWindow(cxt->display, DefaultRootWindow(cxt->display), 
                                 appWindow->x, appWindow->y, appWindow->width, appWindow->height, 0, 0L,
                                 WhitePixel(cxt->display, 0));
#endif
    memset(&gcv, 0, sizeof(gcv));
    appWindow->gc = XCreateGC (cxt->display, cxt->pixmap/*appWindow->handle*/, GCGraphicsExposures, &gcv);

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
    XFreeGC(cxt->display, appWindow->gc);
    XDestroyWindow(cxt->display, appWindow->handle);
}

static AppContext *context = NULL;
void init_display() {
    context = app_context_new();
    get_pixmap_info(context);
    context->pixmap = XCreatePixmap(context->display, DefaultRootWindow(context->display), 1366, 768, context->depth);
}

void draw_window(char *fileName) {
    fprintf(stdout, "fileName:%s\n", fileName);
    AppWindow appWindow;
    int ax, ay; //pixmap坐标
    int width, height; //copy图像的寬高
    int x, y; //window中图片的坐标
    int bytes_per_line;
    sscanf(fileName, "tmp/%u_%u_%u_%u_%u_%u_%u_%u_%u_%u_%u",
        &appWindow.x, &appWindow.y, &appWindow.width, &appWindow.height, &ax, &ay, &width, &height, &x, &y, &bytes_per_line);
    fprintf(stdout, "%d %d %d %d %d %d %d\n", ax, ay, width, height, x, y, bytes_per_line); //使用sscanf从格式化的字符串中读取整数
    init_app_window(context, &appWindow);
    XClearWindow(context->display, appWindow.handle);
    XMapWindow(context->display, appWindow.handle);
    XMoveWindow(context->display, appWindow.handle, appWindow.x, appWindow.y);
    
    char *buf = (char *)malloc(width*height*4);
    FILE *fp = fopen(fileName, "r");
    fread(buf, 1, width*height*4, fp);
    printf("buf:%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
    XImage *image = NULL;
    image = XCreateImage (context->display,
			context->visual/*CopyFromParent*/, context->depth,
			ZPixmap, 0,
			buf,
			width, height,
			context->scanline_pad, bytes_per_line
		);
    if (image==NULL) {
        fprintf(stderr, "XCreateImage error!\n");
        return;
    }

    XEvent event;
    while (1) {
        XNextEvent(context->display, &event); //获取事件
        if (event.type == Expose) {
            /* !!!!!等待Expose后再复制数据，否则可能出现显示不出图像的问题 !!!!! */
            XPutImage(context->display, context->pixmap, appWindow.gc,
			        image, 0, 0, ax, ay, width, height);
            XCopyArea(context->display, context->pixmap, appWindow.handle, appWindow.gc,
                    ax, ay, width, height, x, y);
        } else if (event.type==ButtonPress) {
            fprintf(stdout, "ButtonPress\n");
            break;  
        }
    }
    
    XFree(image);
    free(buf);
    fclose(fp);
    destory_app_window(context, &appWindow);
}

void destroy_display() {
    app_context_destroy(context);
    context = NULL;
}

/**
 * fileName:appWindow->x, appWindow->y, appWindow->width, appWindow->height, ax, ay, width, height, x, y, image->bytes_per_line,  (UINT32)tv.tv_sec, (UINT32)tv.tv_usec, (UINT32)appWindow->handle
 */
void draw_tmp_file(char *fileName) {
    init_display();
    draw_window(fileName);
    destroy_display();
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
    Visual *visual;
    GC gc;
    int input_mask;
    int format = ZPixmap;
    int depth = 24;
    int scanline_pad = 32;
    int offset = 0;
    int bytes_per_line = 0;
    int windowX, windowY;
    int windowW, windowH;
    int ax, ay; //pixmap坐标
    int width, height; //copy图像的寬高
    int x, y; //window中图片的坐标
    sscanf(argv[1], "tmp/%u_%u_%u_%u_%u_%u_%u_%u_%u_%u_%u",
        &windowX, &windowY, &windowW, &windowH, &ax, &ay, &width, &height, &x, &y, &bytes_per_line);
    fprintf(stdout, "%d %d %d %d %d %d %d\n", ax, ay, width, height, x, y, bytes_per_line); //使用sscanf从格式化的字符串中读取整数

    display = XOpenDisplay(NULL);
    screen_number = DefaultScreen(display);
    screen = ScreenOfDisplay(display, screen_number);
    visual = DefaultVisual(display, screen_number);
    window = XCreateWindow(display, RootWindowOfScreen(screen),
						windowX, windowY,
						windowW, windowH,
						0,
						depth,
						InputOutput, visual,
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
    XMoveWindow(display, window, windowX, windowY);
    XFlush(display);
    pixmap = XCreatePixmap(display, window, 1366, 768, depth);
    gc = XCreateGC (display, window, 0, NULL); 

    char *buf = (char *)malloc(width*height*4);
    FILE *fp = fopen(argv[1], "r");
    fread(buf, 1, width*height*4, fp);
    XImage *image = XCreateImage (display,
			visual, depth,
			ZPixmap, 0,
			buf,
			width, height,
			scanline_pad, bytes_per_line
		);
    

    XEvent event;
    while (1) {
        XNextEvent(display, &event); //获取事件
        if (event.type == Expose) {
            XPutImage(display, pixmap, gc,
			        image, 0, 0, ax, ay, width, height);
            XCopyArea(display, pixmap, window, gc,
                    ax, ay, width, height, x, y);
        } else if (event.type==ButtonPress) {
            fprintf(stdout, "ButtonPress\n");
            break;  
        }
    }

    XFree(image);
    free(buf);
    fclose(fp);
    XCloseDisplay(display);    
    return 0;
}
#endif
#endif

