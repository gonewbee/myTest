# Compiler/Linker
CC = gcc
LD = gcc

# compiler/linker flags
CFLAGS = -Wall
LDFLAGS = -L/usr/X11R6/lib -lX11

# files removal
RM = /bin/rm -f

# programs' object files
S_WIN_OBJS = simple-window.o
S_DRAW_OBJS = simple-drawing.o
EVENTS_OBJS = events.o
S_TEXT_OBJS = simple-text.o
F_TEXT_OBJS = formated-text.o
S_WM_HINTS_OBJS = simple-wm-hints.o
WIN_OPS_OBJS = window-operations.o
COLOR_DRAW_OBJS = color-drawing.o
DRAW_PIXMAP_OBJS = draw-pixmap.o
CURSOR_OBJS = cursor.o

ALL_OBJS = $(S_WIN_OBJS) $(S_DRAW_OBJS) $(EVENTS_OBJS) $(S_TEXT_OBJS) \
		$(S_WM_HINTS_OBJS) $(WIN_OPS_OBJS) $(COLOR_DRAW_OBJS) \
		$(DRAW_PIXMAP_OBJS) $(CURSOR_OBJS) $(F_TEXT_OBJS)

# programs' executables
S_WIN = simple-window
S_DRAW = simple-drawing
EVENTS = events
S_TEXT = simple-text
F_TEXT = formated-text
S_WM_HINTS = simple-wm-hints
WIN_OPS = window-operations
COLOR_DRAW = color-drawing
DRAW_PIXMAP = draw-pixmap
CURSOR = cursor

ALL_PROGS = $(S_WIN) $(S_DRAW) $(EVENTS) $(S_TEXT) $(S_WM_HINTS) $(WIN_OPS) \
		$(COLOR_DRAW) $(DRAW_PIXMAP) $(CURSOR) $(F_TEXT)

# top-level rule
#all: $(S_WIN) $(S_DRAW) $(EVENTS) $(S_TEXT) $(S_WM_HINTS) $(WIN_OPS) $(COLOR_DRAW) $(DRAW_PIXMAP) $(CURSOR)
all: $(ALL_PROGS)
	 find . \( ! -type d -a -perm +100  \) -exec strip {} \;

$(S_WIN): $(S_WIN_OBJS)
	$(LD) $(LDFLAGS) $(S_WIN_OBJS) -o $(S_WIN)

$(S_DRAW): $(S_DRAW_OBJS)
	$(LD) $(LDFLAGS) $(S_DRAW_OBJS) -o $(S_DRAW)

$(EVENTS): $(EVENTS_OBJS)
	$(LD) $(LDFLAGS) $(EVENTS_OBJS) -o $(EVENTS)

$(F_TEXT): $(F_TEXT_OBJS)
	$(LD) $(LDFLAGS) $(F_TEXT_OBJS) -o $(F_TEXT)

$(S_TEXT): $(S_TEXT_OBJS)
	$(LD) $(LDFLAGS) $(S_TEXT_OBJS) -o $(S_TEXT)

$(S_WM_HINTS): $(S_WM_HINTS_OBJS)
	$(LD) $(LDFLAGS) $(S_WM_HINTS_OBJS) -o $(S_WM_HINTS)

$(S_WIN_OPS): $(S_WIN_OPS_OBJS)
	$(LD) $(LDFLAGS) $(S_WIN_OPS_OBJS) -o $(S_WIN_OPS)

$(COLOR_DRAW): $(COLOR_DRAW_OBJS)
	$(LD) $(LDFLAGS) $(COLOR_DRAW_OBJS) -o $(COLOR_DRAW)

$(DRAW_PIXMAP): $(DRAW_PIXMAP_OBJS)
	$(LD) $(LDFLAGS) $(DRAW_PIXMAP_OBJS) -o $(DRAW_PIXMAP)

$(CURSOR): $(CURSOR_OBJS)
	$(LD) $(LDFLAGS) $(CURSOR_OBJS) -o $(CURSOR)

# compile C source files into object files.
%.o: %.c 
	$(CC) $(CFLAGS) -c $<

# clean everything
clean:
	$(RM) $(ALL_OBJS) $(ALL_PROGS) core *~

