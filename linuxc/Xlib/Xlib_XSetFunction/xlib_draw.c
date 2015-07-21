/**
 * @brief 使用XSetFunction为GC设置function，测试效果
 * 
 * 输入s显示图片，输入1~9或a~f设置GC，并显示图片，输入r恢复显示，q退出
 * 编译：gcc xlib_draw.c xlib_jpeg.c  -lX11 -ljpeg -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
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
    Atom _NET_WM_STATE_SKIP_PAGER;
    Atom _NET_WM_STATE_SKIP_TASKBAR;
    Atom _MOTIF_WM_HINTS;
    Atom _NET_WM_MOVERESIZE;
    Atom WM_DELETE_WINDOW;
    Atom _NET_WM_WINDOW_TYPE;
	XSetWindowAttributes winAttrib;
    int primary_x;  
    int primary_y;
    int primary_width;
    int primary_height;
    Visual* visual;
}AppContext;

typedef struct _app_window {
    Window handle;
    int x;
    int y;
    int width;
    int height;
    GC gc;
    Window parentWindow;
    int is_transient; /**< 是否是瞬态窗口 */
    int windowId; /**< XSetClassHint时使用 */
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

/**
 * @breif 创建实例
 */
AppContext *app_context_new() {
    AppContext *cxt = (AppContext *)malloc(sizeof(AppContext));
    cxt->display = XOpenDisplay(NULL);
    cxt->_NET_WM_STATE = XInternAtom(cxt->display, "_NET_WM_STATE", False);
	cxt->_NET_WM_STATE_SKIP_PAGER = XInternAtom(cxt->display, "_NET_WM_STATE_SKIP_PAGER", False);
	cxt->_NET_WM_STATE_SKIP_TASKBAR = XInternAtom(cxt->display, "_NET_WM_STATE_SKIP_TASKBAR", False);
	cxt->_MOTIF_WM_HINTS = XInternAtom(cxt->display, "_MOTIF_WM_HINTS", False);
    cxt->_NET_WM_MOVERESIZE = XInternAtom(cxt->display, "_NET_WM_MOVERESIZE", False);
    cxt->WM_DELETE_WINDOW = XInternAtom(cxt->display, "WM_DELETE_WINDOW", False);
    cxt->_NET_WM_WINDOW_TYPE = XInternAtom(cxt->display, "_NET_WM_WINDOW_TYPE", False);
    cxt->screen_number = DefaultScreen(cxt->display);
    cxt->screen = ScreenOfDisplay(cxt->display, cxt->screen_number);
    cxt->depth = DefaultDepthOfScreen(cxt->screen);
    cxt->primary = XCreatePixmap(cxt->display, RootWindowOfScreen(cxt->screen), 1366, 768, cxt->depth);
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
 * @brief 设置后任务栏没有图表
 */
void xf_SetWindowUnlisted(AppContext *cxt, Window window)
{
	Atom window_state[2];
    fprintf(stdout, "===============xf_SetWindowUnlisted\n");
	window_state[0] = cxt->_NET_WM_STATE_SKIP_PAGER;
	window_state[1] = cxt->_NET_WM_STATE_SKIP_TASKBAR;

	XChangeProperty(cxt->display, window, cxt->_NET_WM_STATE,
			XA_ATOM, 32, PropModeReplace, (char *) &window_state, 2);
}

/**
 * @brief 设置图标名称
 */
void xf_SetWindowText(AppContext* xfc, AppWindow* appWindow, char* name)
{
	fprintf(stdout, "xf_SetWindowText::::::::%s\n", name);
	XStoreName(xfc->display, appWindow->handle, name);
	const size_t i = strlen(name);
	XStoreName(xfc->display, appWindow->handle, name);

	Atom wm_Name = XInternAtom(xfc->display, "_NET_WM_NAME", False);
	Atom utf8Str = XInternAtom(xfc->display, "UTF8_STRING", False);

	XChangeProperty(xfc->display, appWindow->handle, wm_Name, utf8Str, 8,
	                PropModeReplace, (unsigned char *)name, i);
}

void xf_SetWindowPID(AppContext* xfc, Window window, pid_t pid)
{
	Atom am_wm_pid;

	if (!pid)
		pid = getpid();

	am_wm_pid = XInternAtom(xfc->display, "_NET_WM_PID", False);

	XChangeProperty(xfc->display, window, am_wm_pid, XA_CARDINAL,
				32, PropModeReplace, (unsigned char*) &pid, 1);
}

/**
 * @breif 创建窗口
 */
void create_window(AppContext *cxt, AppWindow *appWindow) {
	int input_mask;
    XGCValues gcv;
	XWMHints* InputModeHint;
    XClassHint* class_hints;

	appWindow->handle = XCreateWindow(cxt->display, appWindow->parentWindow,
						appWindow->x, appWindow->y,
						appWindow->width, appWindow->height,
						0,
						cxt->depth,
						InputOutput, cxt->visual/*DefaultVisual(cxt->display, 0)*/,
						0, &cxt->winAttrib);
	setWindowDecorations(cxt, appWindow->handle, 0);	//设置后没有Ubuntu自带的关闭、最小和最大这三个键
    memset(&gcv, 0, sizeof(gcv));
    appWindow->gc = XCreateGC (cxt->display, appWindow->handle, GCGraphicsExposures, &gcv); 
#if 1
    //如果两个窗口的res_class和res_name相同的话，窗口的图标在一起，和打开两个终端，终端在任务栏中的图标相似	
    class_hints = XAllocClassHint();
    if (class_hints)
	{
		char* class = NULL;
		
		class = malloc(sizeof("RAIL:00000000"));
		snprintf(class, sizeof("RAIL:00000000"), "RAIL:%08X", appWindow->windowId);
		class_hints->res_class = class;

		class_hints->res_name = "RAIL";
		XSetClassHint(cxt->display, appWindow->handle, class_hints);
		XFree(class_hints);

		free(class);
	}

    /* Set the input mode hint for the WM */
	InputModeHint = XAllocWMHints();
	InputModeHint->flags = (1L << 0);
	InputModeHint->input = True;

	XSetWMHints(cxt->display, appWindow->handle, InputModeHint);
	XFree(InputModeHint);

	XSetWMProtocols(cxt->display, appWindow->handle, &(cxt->WM_DELETE_WINDOW), 1);
#endif
	input_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
				 ButtonReleaseMask | EnterWindowMask | LeaveWindowMask |
				 PointerMotionMask | Button1MotionMask | Button2MotionMask |
				 Button3MotionMask | Button4MotionMask | Button5MotionMask |
				 ButtonMotionMask | KeymapStateMask | ExposureMask |
				 VisibilityChangeMask | StructureNotifyMask | SubstructureNotifyMask |
				 SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
				 ColormapChangeMask | OwnerGrabButtonMask;
	XSelectInput(cxt->display, appWindow->handle, input_mask); //设置要响应的事件
    if (appWindow->is_transient) {
        xf_SetWindowUnlisted(cxt, appWindow->handle);      //设置后任务栏中没有图表
    }
#if 0
    xf_SetWindowPID(cxt, appWindow->handle, 0);
    sendClientEvent(cxt, appWindow->handle, cxt->_NET_WM_STATE, 4, 0,
					XInternAtom(cxt->display, "_NET_WM_STATE_MAXIMIZED_VERT", False),
					XInternAtom(cxt->display, "_NET_WM_STATE_MAXIMIZED_HORZ", False), 0);
#endif

    XClearWindow(cxt->display, appWindow->handle);
	XMapWindow(cxt->display, appWindow->handle);
    XMoveWindow(cxt->display, appWindow->handle, appWindow->x, appWindow->y);
}

/**
 * @brief 读取图片数据并创建XImage，在使用XPutImage将XImage内容复制到Pixmap中，使用XCopyArea显示Pixmap中的内容
 */
void updatePixmap(AppContext *cxt, AppWindow *appWindow, char *fileName, int x, int y) {
    int imageWidth, imageHeight;
    XImage *image;
    u_char *buf = decode_jpeg (fileName, &imageWidth, &imageHeight);
    image = create_image_from_buffer (cxt->display, cxt->screen_number, buf, imageWidth, imageHeight);
    cxt->primary_x = x;
    cxt->primary_y = y;
    cxt->primary_width = imageWidth;
    cxt->primary_height = imageHeight;
    XPutImage(cxt->display, cxt->primary, appWindow->gc,
			image, 0, 0, x, y, imageWidth, imageHeight);
    XFree(image);
}

static const unsigned char xf_rop2_table[] =
{
	0,
	GXclear,        /* 0 */
	GXnor,          /* DPon */
	GXandInverted,  /* DPna */
	GXcopyInverted, /* Pn */
	GXandReverse,   /* PDna */
	GXinvert,       /* Dn */
	GXxor,          /* DPx */
	GXnand,         /* DPan */
	GXand,          /* DPa */
	GXequiv,        /* DPxn */
	GXnoop,         /* D */
	GXorInverted,   /* DPno */
	GXcopy,         /* P */
	GXorReverse,    /* PDno */
	GXor,           /* DPo */
	GXset           /* 1 */
};

int main(int argc, char *argv[]) {
	XEvent event;
	
    AppContext *cxt = app_context_new();
    get_pixmap_info(cxt);
    AppWindow appWindow;
    appWindow.x = 300;
    appWindow.y = 200;
    appWindow.width = 500;
    appWindow.height = 400;
    appWindow.parentWindow = RootWindowOfScreen(cxt->screen);
    appWindow.is_transient = 0;
    appWindow.windowId = 0x12345;
    create_window(cxt, &appWindow);
    xf_SetWindowText(cxt, &appWindow, "Hello World");
	
	XFlush(cxt->display);
    int x=0, y=0;
    KeySym keysym;
    AppWindow *eventWindow;
	while (1) {
		XNextEvent(cxt->display, &event); //获取事件
        if (event.xany.window==appWindow.handle) {
            eventWindow = &appWindow;
        } else {
            fprintf(stdout, "event in unkonwn window\n");
            continue;
        }
		if (event.type!=MotionNotify)
			fprintf(stdout, "event type:%d;%s\n", event.type, X11_EVENT_STRINGS[event.type]);
        switch (event.type) {
            case ButtonPress:
                /* 在窗口区域内按住鼠标左键进行拖动 */
                XUngrabPointer(cxt->display, CurrentTime); //必须要有这句，没有这句不会产生拖动的效果。
                Window child_window;
                XTranslateCoordinates(cxt->display, eventWindow->handle, 
                        RootWindowOfScreen(cxt->screen), event.xbutton.x, event.xbutton.y,
                        &x, &y, &child_window); /**< 加上这句解决鼠标拖动窗体时鼠标位置不对的问题 */
                fprintf(stdout, "x:%d;y:%d;%d;%d\n", x, y, event.xbutton.x, event.xbutton.y);
                sendClientEvent(cxt, eventWindow->handle, cxt->_NET_WM_MOVERESIZE, 5, x, y, 8, 1, 1);
                break;
            case KeyPress:
                keysym = XLookupKeysym (&event.xkey, 0);
                fprintf(stdout, "KeyPress:%x\n", (unsigned int)keysym);
                if (XK_m == keysym) {
                    /* 窗口移动 */
                    XMoveWindow(cxt->display, eventWindow->handle, 100, 50);
                } else if (XK_i == keysym) {
                    /* 最小化 */
                    XIconifyWindow(cxt->display, eventWindow->handle, cxt->screen_number);
                    XFlush(cxt->display);
                    sleep(3);
                    //等待3s恢复窗口显示
                    x11_restore_window(cxt, eventWindow);
                } else if (XK_s == keysym) {
                    updatePixmap(cxt, &appWindow, "test2.jpg", appWindow.x, appWindow.y);
                    XCopyArea(cxt->display, cxt->primary, appWindow.handle, appWindow.gc,
			                appWindow.x, appWindow.y, appWindow.width, appWindow.height, 0, 0);
                } else if ((XK_1<=keysym)&&(keysym<=XK_9)) {
                    XSetFunction(cxt->display, appWindow.gc, xf_rop2_table[keysym-XK_0]);
                    updatePixmap(cxt, &appWindow, "test.jpg", appWindow.x, appWindow.y);
                    XCopyArea(cxt->display, cxt->primary, appWindow.handle, appWindow.gc,
			                appWindow.x, appWindow.y, cxt->primary_width, cxt->primary_height, 0, 0);
                } else if ((XK_a<=keysym)&&(keysym<=XK_f)) {
                    int t = keysym-XK_a+10;
                    fprintf(stdout, "------------------input a~f:%d\n", t);
                    XSetFunction(cxt->display, appWindow.gc, xf_rop2_table[t]);
                    updatePixmap(cxt, &appWindow, "test.jpg", appWindow.x, appWindow.y);
                    XCopyArea(cxt->display, cxt->primary, appWindow.handle, appWindow.gc,
			                appWindow.x, appWindow.y, cxt->primary_width, cxt->primary_height, 0, 0);
                } else if (XK_r == keysym) {
                    XSetFunction(cxt->display, appWindow.gc, GXcopy);
                    updatePixmap(cxt, &appWindow, "test.jpg", appWindow.x, appWindow.y);
                    XCopyArea(cxt->display, cxt->primary, appWindow.handle, appWindow.gc,
			                appWindow.x, appWindow.y, cxt->primary_width, cxt->primary_height, 0, 0);
                }  else if (XK_q == keysym) {
                    fprintf(stdout, "try to quit\n");
                    exit(0);
                }
                break;
            case FocusIn:
                if (eventWindow->handle==appWindow.handle) {
                    //x11_restore_window(cxt, &childWindow);//显示出子窗口
                }
                break;
        }
	}
	
	XDestroyWindow(cxt->display, appWindow.handle);
	XCloseDisplay(cxt->display);
    free(cxt);
    return 0;
}

