#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static Atom XA_TARGETS;
static Atom XA_multiple;
static char *txt = "Hello, World!";
static int len;
Atom mytarget = XA_STRING;

static char* GetAtomName(Display* disp, Atom a)
{
	if(a == None)
		return "None";
	else
		return XGetAtomName(disp, a);
}

static void set_targets_property(Display* disp, Window w, Atom typed_data, Atom property)
{

	Atom types[3] = { XA_TARGETS, XA_multiple, typed_data };

	//Fill up this property with a list of targets.
	XChangeProperty(disp, w, property, XA_ATOM, 32, 
						PropModeReplace, (unsigned char *) types,
					    (int) (sizeof(types) / sizeof(Atom)));
}

static void process_selection_request(XEvent e) {
	if(e.type != SelectionRequest)
		return;

	//Extract the relavent data
	Window owner     = e.xselectionrequest.owner;
	Atom selection   = e.xselectionrequest.selection;
	Atom target      = e.xselectionrequest.target;
	Atom property    = e.xselectionrequest.property;
	Window requestor = e.xselectionrequest.requestor;
	Time timestamp   = e.xselectionrequest.time;
	Display* disp    = e.xselection.display;

	fprintf(stdout, "A selection request has arrived!\n");
	fprintf(stdout, "Owner = 0x%x\n", (int)owner);
	fprintf(stdout, "Selection atom = %s\n", GetAtomName(disp, selection));
	fprintf(stdout, "Target atom    = %s\n", GetAtomName(disp, target));
	fprintf(stdout, "Property atom  = %s\n", GetAtomName(disp, property));	

	//X should only send requests for the selections since we own.
	//since we own exaclty one, we don't need to check it.

	//Replies to the application requesting a pasting are XEvenst
	//sent via XSendEvent
	XEvent s;

	//Start by constructing a refusal request.
	s.xselection.type = SelectionNotify;
	//s.xselection.serial     - filled in by server
	//s.xselection.send_event - filled in by server
	//s.xselection.display    - filled in by server
	s.xselection.requestor = requestor;
	s.xselection.selection = selection;
	s.xselection.target    = target;
	s.xselection.property  = None;   //This means refusal
	s.xselection.time      = timestamp;



	if(target ==XA_TARGETS)
	{
		fprintf(stdout, "Replying with a target list\n");
		set_targets_property(disp, requestor, mytarget, property);
		s.xselection.property = property;
	}
	else if(mytarget==target)
	{
		//We're asked to convert to one the formate we know about
		fprintf(stdout, "Replying with which ever data I have\n");

		//Fill up the property with the URI.
		s.xselection.property = property;
		XChangeProperty(disp, requestor, property, target, 8, PropModeReplace, 
						txt, len);
	}
	else
	{	
		//We've been asked to converto to something we don't know 
		//about.
		fprintf(stdout, "No valid conversion. Replying with refusal\n");
	}
	
	//Reply
	XSendEvent(disp, e.xselectionrequest.requestor, True, 0, &s);
	fprintf(stdout, "\n");
}

int main(int argc, char const *argv[]) {
	Display* dis;
	Window win;
	XEvent event;
	// Atom target = XA_STRING;
	Atom pty;
	int ret = 0;
	dis = XOpenDisplay(NULL);
	len = strlen(txt);
	// 窗口不用显示，所以只用创建窗口不用map
	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 1, 1, 0, 0, 0);
	Atom sel = XInternAtom(dis, "CLIPBOARD", False);
	if (sel==None) {
		fprintf(stderr, "XInternAtom CLIPBOARD error\n");
		return 1;
	}
	XA_TARGETS = XInternAtom(dis, "TARGETS", False);
	XA_multiple = XInternAtom(dis, "MULTIPLE", False);

	XSetSelectionOwner(dis, sel, win, CurrentTime);
	XFlush(dis);
	while (1) {
		XNextEvent(dis, &event);
		fprintf(stdout, "event type:%d\n", event.type);
		if (event.type==SelectionRequest) {
#if 1
			process_selection_request(event);
#else			
			win = event.xselectionrequest.requestor;
			pty = event.xselectionrequest.property;
			fprintf(stdout, "get %s\n", XGetAtomName(dis, event.xselectionrequest.target));
			if (event.xselectionrequest.target==XA_TARGETS) {
				fprintf(stdout, "XA_TARGETS==\n");
				Atom types[2] = { XA_TARGETS, mytarget };
				XChangeProperty(dis,
					    win,
					    pty,
					    XA_ATOM,
					    32, PropModeReplace, (unsigned char *) types,
					    (int) (sizeof(types) / sizeof(Atom))
				);
			} else {
				/* send data all at once (not using INCR) */
			    XChangeProperty(dis,
					    win,
					    pty, mytarget, 8, PropModeReplace, (unsigned char *) txt, (int) len);
			    ret = 1;
			}
			XEvent res;			/* response to event */
			/* set values for the response event */
			res.xselection.property = pty;
			res.xselection.type = SelectionNotify;
			res.xselection.display = event.xselectionrequest.display;
			res.xselection.requestor = win;
			res.xselection.selection = event.xselectionrequest.selection;
			res.xselection.target = event.xselectionrequest.target;
			res.xselection.time = event.xselectionrequest.time;

			/* send the response event */
			XSendEvent(dis, event.xselectionrequest.requestor, 0, 0, &res);
			XFlush(dis);
			if (ret)
				return 0;
#endif
		} else if (event.type==SelectionClear) {
			fprintf(stdout, "SelectionClear received\n");
			return 0;
		}
	}
	return 0;
}
