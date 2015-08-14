/**
 * @file draw.c
 * @brief 使用X11接口绘制线段、矩形
 */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int main(int argc, char *argv[]) {
	Display *dsp = XOpenDisplay(NULL);
	if (!dsp) {
		fprintf(stderr, "XOpenDisplay error!\n");
		return 1;
	}
	int input_mask;
	XEvent event;
	int screenNumber = DefaultScreen(dsp);
	unsigned long white = WhitePixel(dsp, screenNumber);
	unsigned long black = BlackPixel(dsp, screenNumber);
	int x = 200;
	int y = 100;
	int width = 300;
	int height = 400;
	int depth = XDefaultDepth(dsp, screenNumber); //颜色多少位
	fprintf(stdout, "screenNumber:%d,depth:%d\n", screenNumber, depth);
	fprintf(stdout, "black:0x%lx\n", black);
	fprintf(stdout, "white:0x%lx\n", white);
	Window win = XCreateSimpleWindow(dsp, 
									DefaultRootWindow(dsp),
									x, y,	/* 位置 */
									width, height, /* 宽高 */
									0, /* border_width */
									black, /* border */
									white /* background */);
	GC gc = XCreateGC(dsp, win,
						0,
						NULL); /* 申请Graphics Context */
	XStoreName(dsp, win, "draw test"); /* 和Window Manager，设置窗口名称 */
	input_mask = KeyPressMask | KeyReleaseMask;
	XSelectInput(dsp, win, input_mask);
	XMapWindow(dsp, win);
	XMoveWindow(dsp, win, x, y);
	KeySym keysym;
	unsigned long color = 0;
	while(1) {
		XNextEvent(dsp, &event);
		switch(event.type) {
		case KeyPress:
			keysym = XLookupKeysym (&event.xkey, 0);
			switch(keysym) {
			case XK_1:
				fprintf(stdout, "XK_1 input, draw line\n");
				color = 0xFF0000;
				// XSetFillStyle(dsp, gc, FillSolid);
				XSetForeground(dsp, gc, color); //设置颜色
				XDrawLine(dsp, win, gc, 10, 10, 190, 190); /* 画线段，from-to */
				color = 0xFF00;
				XSetForeground(dsp, gc, color); //设置颜色
				XDrawLine(dsp, win, gc, 30, 10, 210, 190); /* 画线段，from-to */
				break;
			case XK_2:
				fprintf(stdout, "XK_2 input, fill rectangle\n");
				// XSetFillStyle(dsp, gc, FillSolid);
				color = 0xFF;
				XSetForeground(dsp, gc, color); //设置颜色
				XFillRectangle(dsp, win, gc, 10, 20, 30, 50); //绘制矩形，并填充
				break;
			case XK_3:
				fprintf(stdout, "XK_3 input, draw rectangle\n");
				// XSetFillStyle(dsp, gc, FillSolid);
				color = 0xFFF00;
				XSetForeground(dsp, gc, color); //设置颜色
				XDrawRectangle(dsp, win, gc, 30, 100, 40, 60); //绘制矩形，不填充
				break;
			case XK_4:
                //绘制折线
				fprintf(stdout, "XK_4 input, draw polyline\n");
				XPoint points[5] = {0}; //如果开头和结尾点重回形成闭合图形
				int npoints = 5;
				points[0].x = 10;
				points[0].y = 10;
				points[1].x = 0;
				points[1].y = 90;
				points[2].x = 90;
				points[2].y = 0;
				points[3].x = 0;
				points[3].y = -90;
				points[4].x = -90;
				points[4].y = 0;
				color = 0xA757A8; //紫色
				XSetForeground(dsp, gc, color); //设置颜色
                //相比前一个点坐标的变化值，不是点坐标的绝对数值
				XDrawLines(dsp, win, gc, points, npoints, CoordModePrevious);
				break;
			case XK_5:
                //绘制多边形
				fprintf(stdout, "XK_5 input, fill polygon\n");
				XPoint ppoints[4] = {0};
				int nppoints = 4;
				ppoints[0].x = 100;
				ppoints[0].y = 120;
				ppoints[1].x = 40;
				ppoints[1].y = 35;
				ppoints[2].x = -20;
				ppoints[2].y = 60;
				ppoints[3].x = 0;
				ppoints[3].y = -90;
				color = 0xA757A8; //紫色
				XSetForeground(dsp, gc, color); //设置颜色
				XFillPolygon(dsp, win, gc, ppoints, nppoints, Complex, CoordModePrevious);
				break;
			case XK_q:
				fprintf(stdout, "q input, quit\n");
				goto quit;
			}
			break;
		}
	}
quit:
	XDestroyWindow(dsp, win);
	XCloseDisplay(dsp);
	return 0;
}
