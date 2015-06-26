/**
 * @file draw_line.c
 * @brief X11测试
 *
 * 编译：gcc draw_line.c -lX11
 * 也可以：gcc draw_line.c `pkg-config --cflags --libs x11`
 * 调试编译，需要按任意键继续：gcc draw_line.c -lX11 -D_DEBUG_WAIT
 */
#include <stdio.h>
#include <X11/Xlib.h>
#include <string.h>

#ifdef _DEBUG_WAIT
#define debug_wait(x)	do{getchar();}while(0)
#else
#define debug_wait(x)
#endif

int main(int argc, char *argv[]) {
    Display *dsp = XOpenDisplay(NULL);
	if (!dsp) {
		fprintf(stderr, "XOpenDisplay error!\n");
		return 1;
	}
	
	int screenNumber = DefaultScreen(dsp);
	unsigned long white = WhitePixel(dsp, screenNumber);
	unsigned long black = BlackPixel(dsp, screenNumber);
	
	Window win = XCreateSimpleWindow(dsp, 
									DefaultRootWindow(dsp),
									50, 50,	/* 位置 */
									200, 300, /* 宽高 */
									0, /* border_width */
									black, /* border */
									white /* background */);
	
	XStoreName(dsp, win, "hello world"); /* 和Window Manager，设置窗口名称 */
	fprintf(stderr, "Map window\n");
	XMapWindow(dsp, win);
	debug_wait(); /* 至此已执行Map动作，但显示器上没有变化 */
	fprintf(stderr, "XFlush\n");
	XFlush(dsp);
	debug_wait(); /* XFlush后可以看到窗口 */
	
	GC gc = XCreateGC(dsp, win,
						0,
						NULL); /* 申请Graphics Context */
	XSetForeground(dsp, gc, black); /* 设置前景颜色，XSetBackground设置背景颜色 */
	
	/* 画线 */
	fprintf(stderr, "Draw Line\n");
	XDrawLine(dsp, win, gc, 10, 10, 190, 190); /* 画线，from-to */
	XSetLineAttributes(dsp, gc, 5, LineOnOffDash, CapButt, JoinRound); /* 设置线样式 */
	XDrawLine(dsp, win, gc, 10, 190, 190, 10);
	debug_wait();
	XFlush(dsp);
	
	/* 显示文字 */
	char *str = "Hello World";
	fprintf(stderr, "Draw String\n");
	XDrawString(dsp, win, gc, 30, 200, str, strlen(str));
	debug_wait();
	XFlush(dsp);
	
	getchar();
	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);
	return 0;
}
