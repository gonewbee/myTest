/**
 * @brief 测试图片显示
 * 
 * 编译：gcc xlib_draw.c xlib_jpeg.c  -lX11 -ljpeg -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include "xlib_jpeg.h"

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_FUNC_ALL            (1L << 0)
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define PROP_MOTIF_WM_HINTS_ELEMENTS	5

/** 事件名称 */
const char* const X11_EVENT_STRINGS[] =
{
	"", "",
	"KeyPress",
	"KeyRelease",
	"ButtonPress",
	"ButtonRelease",
	"MotionNotify",
	"EnterNotify",
	"LeaveNotify",
	"FocusIn",
	"FocusOut",
	"KeymapNotify",
	"Expose",
	"GraphicsExpose",
	"NoExpose",
	"VisibilityNotify",
	"CreateNotify",
	"DestroyNotify",
	"UnmapNotify",
	"MapNotify",
	"MapRequest",
	"ReparentNotify",
	"ConfigureNotify",
	"ConfigureRequest",
	"GravityNotify",
	"ResizeRequest",
	"CirculateNotify",
	"CirculateRequest",
	"PropertyNotify",
	"SelectionClear",
	"SelectionRequest",
	"SelectionNotify",
	"ColormapNotify",
	"ClientMessage",
	"MappingNotify",
	"GenericEvent",
};

typedef struct _app_context {
    Display *display;
    int screen_number;
    Screen *screen;
    int depth;
    Pixmap primary;
    Atom _NET_WM_STATE;
    Atom _MOTIF_WM_HINTS;
    Atom _NET_WM_MOVERESIZE;
	XSetWindowAttributes winAttrib;
    GC gc;
    int primary_x;
    int primary_y;
    int primary_width;
    int primary_height;
}AppContext;

typedef struct _app_window {
    Window handle;
    int x;
    int y;
    int width;
    int height;
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
 * @breif 创建实例
 */
AppContext *app_context_new() {
    AppContext *cxt = (AppContext *)malloc(sizeof(AppContext));
    cxt->display = XOpenDisplay(NULL);
    cxt->_NET_WM_STATE = XInternAtom(cxt->display, "_NET_WM_STATE", False);
	cxt->_MOTIF_WM_HINTS = XInternAtom(cxt->display, "_MOTIF_WM_HINTS", False);
    cxt->_NET_WM_MOVERESIZE = XInternAtom(cxt->display, "_NET_WM_MOVERESIZE", False);
    cxt->screen_number = DefaultScreen(cxt->display);
    cxt->screen = ScreenOfDisplay(cxt->display, cxt->screen_number);
    cxt->depth = DefaultDepthOfScreen(cxt->screen);
    return cxt;
}

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


/**
 * @breif 模拟发送事件
 *
 * @param atom  _NET_WM_MOVERESIZE 发送这个后就可以按住鼠标拖动窗体
 *              _NET_WM_STATE 窗口状态，如最大化
 */
void sendClientEvent(AppContext *cxt, Window window, Atom atom, unsigned int numArgs, ...) {
    XEvent xevent;
    unsigned int i;
    va_list argp;
    va_start(argp, numArgs);
    memset(&xevent, 0, sizeof(XEvent));

    xevent.xclient.type = ClientMessage;
    xevent.xclient.serial = 0;
    xevent.xclient.send_event = False;
    xevent.xclient.display = cxt->display;
    xevent.xclient.window = window;
    xevent.xclient.message_type = atom;
    xevent.xclient.format = 32;

    fprintf(stdout, "%s numArgs:%d\n", __func__, numArgs);
    for (i=0; i<numArgs; i++) {
        xevent.xclient.data.l[i] = va_arg(argp, int);
    }
    XSendEvent(cxt->display, RootWindowOfScreen(cxt->screen), False,
             SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
    XSync(cxt->display, False);
    va_end(argp);
}

/**
 * @brief 恢复窗口显示
 */
void x11_restore_window(AppContext *cxt, AppWindow *appWindow) {
    XClientMessageEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = ClientMessage;
    ev.window = appWindow->handle;
    ev.message_type = XInternAtom(cxt->display, "_NET_ACTIVE_WINDOW", True);
    ev.format = 32;
    ev.data.l[0] = 1;
    ev.data.l[1] = CurrentTime;
    ev.data.l[2] = ev.data.l[3] = ev.data.l[4] = 0;
    XSendEvent (cxt->display, RootWindowOfScreen(cxt->screen), False,
      SubstructureRedirectMask |SubstructureNotifyMask, (XEvent*)&ev);
    XFlush (cxt->display);
}

/**
 * @breif 创建窗口
 */
void create_window(AppContext *cxt, AppWindow *appWindow) {
	appWindow->handle = XCreateWindow(cxt->display, RootWindowOfScreen(cxt->screen),
						appWindow->x, appWindow->y,
						appWindow->width, appWindow->height,
						0,
						cxt->depth,
						InputOutput, DefaultVisual(cxt->display, 0),
						0, &cxt->winAttrib);
	setWindowDecorations(cxt, appWindow->handle, 0);	//设置后没有Ubuntu自带的关闭、最小和最大这三个键
    if (!cxt->gc)
        cxt->gc = XCreateGC (cxt->display, appWindow->handle, 0, NULL); 
    if (!cxt->primary) 
        cxt->primary = XCreatePixmap(cxt->display, appWindow->handle, appWindow->width, appWindow->height, cxt->depth);
}

/**
 * @brief 读取图片数据并创建XImage，在使用XPutImage将XImage内容复制到Pixmap中，使用XCopyArea显示Pixmap中的内容
 */
void updatePixmap(AppContext *cxt, char *fileName, int x, int y) {
    int imageWidth, imageHeight;
    XImage *image;
    u_char *buf = decode_jpeg (fileName, &imageWidth, &imageHeight);
    image = create_image_from_buffer (cxt->display, cxt->screen_number, buf, imageWidth, imageHeight);
    cxt->primary_x = x;
    cxt->primary_y = y;
    cxt->primary_width = imageWidth;
    cxt->primary_height = imageHeight;
    XPutImage(cxt->display, cxt->primary, cxt->gc,
			image, 0, 0, x, y, imageWidth, imageHeight);
    XFree(image);
}

int main(int argc, char *argv[]) {
	int input_mask;
	XEvent event;
	
    AppContext *cxt = app_context_new();
    AppWindow appWindow;
    appWindow.x = 300;
    appWindow.y = 200;
    appWindow.width = 500;
    appWindow.height = 400;
    create_window(cxt, &appWindow);
	
	input_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
				 ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
				 PointerMotionMask | Button1MotionMask | Button2MotionMask |
				 Button3MotionMask | Button4MotionMask | Button5MotionMask |
				 ButtonMotionMask | KeymapStateMask | ExposureMask |
				 VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask |
				 SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
				 ColormapChangeMask | OwnerGrabButtonMask;
	XSelectInput(cxt->display, appWindow.handle, input_mask); //设置要响应的事件
	
	XMapWindow(cxt->display, appWindow.handle);
    XMoveWindow(cxt->display, appWindow.handle, appWindow.x, appWindow.y);
	XFlush(cxt->display);
    int x=0, y=0;
    KeySym keysym;
	while (1) {
		XNextEvent(cxt->display, &event); //获取事件
		if (event.type!=MotionNotify)
			fprintf(stdout, "event type:%d;%s\n", event.type, X11_EVENT_STRINGS[event.type]);
        switch (event.type) {
        case ButtonPress:
            /* 在窗口区域内按住鼠标左键进行拖动 */
            XUngrabPointer(cxt->display, CurrentTime); //必须要有这句，没有这句不会产生拖动的效果。
            Window child_window;
            XTranslateCoordinates(cxt->display, appWindow.handle, 
                    RootWindowOfScreen(cxt->screen), event.xbutton.x, event.xbutton.y,
                    &x, &y, &child_window); /**< 加上这句解决鼠标拖动窗体时鼠标位置不对的问题 */
            fprintf(stdout, "x:%d;y:%d;%d;%d\n", x, y, event.xbutton.x, event.xbutton.y);
            sendClientEvent(cxt, appWindow.handle, cxt->_NET_WM_MOVERESIZE, 5, x, y, 8, 1, 1);
            break;
        case KeyPress:
            keysym = XLookupKeysym (&event.xkey, 0);
            if (XK_h == keysym) {
                fprintf(stdout, "h key press;try to hide\n");
                XWithdrawWindow(cxt->display, appWindow.handle, cxt->screen_number);
            } else if (XK_m == keysym) {
                /* 窗口移动 */
                XMoveWindow(cxt->display, appWindow.handle, 100, 50);
            } else if (XK_i == keysym) {
                /* 最小化 */
                XIconifyWindow(cxt->display, appWindow.handle, cxt->screen_number);
                XFlush(cxt->display);
                sleep(3);
                //等待3s恢复窗口显示
                x11_restore_window(cxt, &appWindow);
            } else if (XK_a == keysym) {
                // 最大化
                //_NET_WM_STATE_MAXIMIZED_{VERT,HORZ} indicates that the window is {vertically,horizontally} maximized
                sendClientEvent(cxt, appWindow.handle, cxt->_NET_WM_STATE, 4, 1, 
                        XInternAtom(cxt->display, "_NET_WM_STATE_MAXIMIZED_VERT", False),
                        XInternAtom(cxt->display, "_NET_WM_STATE_MAXIMIZED_HORZ", False), 0);
            } else if (XK_s == keysym) {
                updatePixmap(cxt, "test.jpg", 0, 0);
                XCopyArea(cxt->display, cxt->primary, appWindow.handle, cxt->gc,
			            cxt->primary_x, cxt->primary_y, cxt->primary_width, cxt->primary_height, cxt->primary_x, cxt->primary_y);
	            XFlush(cxt->display);
                updatePixmap(cxt, "test2.jpg", 50, 50);
                XCopyArea(cxt->display, cxt->primary, appWindow.handle, cxt->gc,
			            cxt->primary_x, cxt->primary_y, cxt->primary_width, cxt->primary_height, cxt->primary_x, cxt->primary_y);
	            XFlush(cxt->display);
            } else if (XK_q == keysym) {
                fprintf(stdout, "try to quit\n");
                exit(0);
            }
        }
	}
	
	XDestroyWindow(cxt->display, appWindow.handle);
	XCloseDisplay(cxt->display);
    free(cxt);
    return 0;
}

