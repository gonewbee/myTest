/**
 * @breif 显示使用XGetImage从pixmap中保存的数据
 *
 * 创建一次窗口，更新这个窗口的图像，显示多次重叠后的图像数据
 * 编译动态库：gcc -o draw_continue.so draw_continue.c -lX11 -fPIC -shared
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
    int winId;
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
    cxt->visual = DefaultVisual(cxt->display, cxt->screen_number);
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
                                 BlackPixel(cxt->display, 0)); //显示黑色背景
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
static AppWindow *appWindow = NULL;
/**
 * @brief 初始化显示资源
 */
void init_display() {
    context = app_context_new();
    appWindow = (AppWindow *)malloc(sizeof(AppWindow));
    memset(appWindow, 0, sizeof(AppWindow));
    //get_pixmap_info(context);
    context->pixmap = XCreatePixmap(context->display, DefaultRootWindow(context->display), 1366, 768, context->depth);
}

/**
 * @brief 显示图像
 */
void draw_window(char *fileName) {
    fprintf(stdout, "fileName:%s\n", fileName);
    int ax, ay; //pixmap坐标
    int width, height; //copy图像的寬高
    int x, y; //window中图片的坐标
    int bytes_per_line;
    int windowX, windowY; //窗口坐标
    int windowW, windowH; //窗口尺寸
    int winId;
    int inited = 0;
    sscanf(fileName, "tmp/%x_%u_%u_%u_%u_%u_%u_%u_%u_%u_%u_%u",
        &winId, &windowX, &windowY, &windowW, &windowH, &ax, &ay, &width, &height, &x, &y, &bytes_per_line);
    fprintf(stdout, "%d %d %d %d %d %d %d\n", ax, ay, width, height, x, y, bytes_per_line); //使用sscanf从格式化的字符串中读取整数
    if (0==appWindow->winId) {
        //初始化窗口
        inited = 1;
        appWindow->x = windowX;
        appWindow->y = windowY;
        appWindow->width = windowW;
        appWindow->height = windowH;
        appWindow->winId = winId;
        init_app_window(context, appWindow);
        XClearWindow(context->display, appWindow->handle);
        XMapWindow(context->display, appWindow->handle);
        XMoveWindow(context->display, appWindow->handle, appWindow->x, appWindow->y);
    } else if (appWindow->winId!=winId) {
        //先只显示一个窗口
        fprintf(stdout, "winId not equal:%x!=%x\n", appWindow->winId, winId);
        return;
    }
    if ((appWindow->x!=windowX) || (appWindow->y!=windowY)) {
        appWindow->x = windowX;
        appWindow->y = windowY;
        XMoveWindow(context->display, appWindow->handle, appWindow->x, appWindow->y);
    }
    
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
    
    XPutImage(context->display, context->pixmap, appWindow->gc,
	        image, 0, 0, ax, ay, width, height);
    if (!inited) {
        XCopyArea(context->display, context->pixmap, appWindow->handle, appWindow->gc,
                    ax, ay, width, height, x, y);
    } else {
        XEvent event;
        while (1) {
            XNextEvent(context->display, &event); //获取事件
            if (event.type == Expose) {
                fprintf(stdout, "event Expose\n");
                /* !!!!!等待Expose后再复制数据，否则可能出现显示不出图像的问题 !!!!! */
                XCopyArea(context->display, context->pixmap, appWindow->handle, appWindow->gc,
                        ax, ay, width, height, x, y);
                break;
            } else if (event.type==ButtonPress) {
                fprintf(stdout, "ButtonPress\n");
                break;  
            }
        }
    }

    
    
    XFree(image);
    free(buf);
    fclose(fp);
}

/**
 * @brief 释放资源
 */
void destroy_display() {
    destory_app_window(context, appWindow);
    free(appWindow);
    app_context_destroy(context);
    context = NULL;
}

