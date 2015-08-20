/**
 * @brief X11 selection测试，将剪切板中的内容打印出来
 * 编译：gcc paste.c -lX11
 * 参考：https://github.com/edrosten/x_clipboard
 */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

typedef struct _property {
	unsigned char *data;
	int format, nitems;
	Atom type;
}Property;

static Atom XA_TARGETS;

static char* GetAtomName(Display* disp, Atom a) {
	if(a == None)
		return "None";
	else
		return XGetAtomName(disp, a);
}

/**
 * @brief 调用XGetWindowProperty，读取Property中的数据
 */
static Property read_property(Display* disp, Window w, Atom property) {
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long bytes_after;
	unsigned char *ret=0;
	
	int read_bytes = 1024;	

	//Keep trying to read the property until there are no
	//bytes unread.
	do
	{
		if(ret != 0)
			XFree(ret);
		XGetWindowProperty(disp, w, property, 0, read_bytes, False, AnyPropertyType,
							&actual_type, &actual_format, &nitems, &bytes_after, 
							&ret);

		read_bytes *= 2;
	}while(bytes_after != 0);
	
	fprintf(stdout, "\n");
	fprintf(stdout, "Actual type: %s\n", GetAtomName(disp, actual_type));
	fprintf(stdout, "Actual format: %d\n", actual_format);
	fprintf(stdout, "Number of items: %ld\n", nitems);

	Property p = {ret, actual_format, nitems, actual_type};

	return p;
}

static void list_targets_name(Display* disp, Property p) {
	Atom *atom_list = (Atom*)p.data;
	int i = 0;
	for(i=0; i < p.nitems; i++) {
		fprintf(stdout, "Type:%d = %s\n", i, GetAtomName(disp, atom_list[i]));
	}
}

int main(int argc, char *argv[]) {
	Display* dis;
	Window win;
	XEvent event;
	int sent_request = 0;
	dis = XOpenDisplay(NULL);
	// 窗口不用显示，所以只用创建窗口不用map
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 1, 1, 0, 0, 0);
	// Use PRIMARY
	// Atom sel = XInternAtom(dis, "PRIMARY", False);
	// 使用 CLIPBOARD 
	Atom sel = XInternAtom(dis, "CLIPBOARD", False);
	XA_TARGETS = XInternAtom(dis, "TARGETS", False);
	Atom to_be_requested = None;

	// 传入TARGETS，获取可用的类型列表
    // 使用XGetWindowProperty读取Property
	XConvertSelection(dis, sel, XA_TARGETS, sel, win, CurrentTime);
	XFlush(dis);
	while(1) {
        // SelectionNotify事件不需要使用XSelectInput进行设置
		XNextEvent(dis, &event);
		fprintf(stdout, "event type:%d\n", event.type);
		if(event.type == SelectionNotify) {
			Atom target = event.xselection.target;
			fprintf(stdout, "A selection notify has arrived!\n");
			fprintf(stdout, "Requestor = 0x%x\n", (int)event.xselectionrequest.requestor);
			fprintf(stdout, "Selection atom = %s\n", GetAtomName(dis, event.xselection.selection));
			fprintf(stdout, "Target atom    = %s\n", GetAtomName(dis, target));
			fprintf(stdout, "Property atom  = %s\n", GetAtomName(dis, event.xselection.property));

			if(event.xselection.property == None) {
				return 2 + (target == XA_TARGETS);
			} else {
				Property prop = read_property(dis, win, sel);
				if(target == XA_TARGETS && !sent_request) {
					sent_request = 1;
					list_targets_name(dis, prop);
					// 从selection中获取XA_STRING类型
					to_be_requested = XA_STRING;
					fprintf(stdout, "Now requsting type %s\n", GetAtomName(dis, to_be_requested));
					// 设置要获取类型的数
					XConvertSelection(dis, sel, to_be_requested, sel, win, CurrentTime);
				} else if(target == to_be_requested) {
					fprintf(stdout, "%s\n", (char*)prop.data);
					return 0;
				}
			}
		}
	}
}
