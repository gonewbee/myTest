CC=cc
CINCLUDE=-I/usr/X11R6/include -I/usr/local/include
CFLAGS=-Wall -g
LFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lX11 -ljpeg -lm

all: Xlib_JPEG_Example
	
Xlib_JPEG_Example: Xlib_JPEG_Example.c
	$(CC) $(CFLAGS) Xlib_JPEG_Example.c $(CINCLUDE) $(LFLAGS) -o Xlib_JPEG_Example

clean:
	rm -f Xlib_JPEG_Example *.o *.core