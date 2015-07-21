#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <jpeglib.h>
#include <jerror.h>

#ifndef u_char
#define u_char unsigned char
#endif		

int get_byte_order (void) {
	union {
		char c[sizeof(short)];
		short s;
	} order;

	order.s = 1;
	if ((1 == order.c[0])) {
		return LSBFirst;
	} else {
		return MSBFirst;
	}
}
		
void jpeg_error_exit (j_common_ptr cinfo) {
	cinfo->err->output_message (cinfo);
	exit (EXIT_FAILURE);
}

/*This returns an array for a 24 bit image.*/
u_char *decode_jpeg (char *filename, int *widthPtr, int *heightPtr) {
	register JSAMPARRAY lineBuf;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr err_mgr;
	int bytesPerPix;
	FILE *inFile;
	u_char *retBuf;
	
	inFile = fopen (filename, "rb");
	if (NULL == inFile) { 
		perror (NULL);
		return NULL;
	}

	cinfo.err = jpeg_std_error (&err_mgr);
	err_mgr.error_exit = jpeg_error_exit;	

	jpeg_create_decompress (&cinfo);
	jpeg_stdio_src (&cinfo, inFile);
	jpeg_read_header (&cinfo, 1);
	cinfo.do_fancy_upsampling = 0;
	cinfo.do_block_smoothing = 0;
	jpeg_start_decompress (&cinfo);

	*widthPtr = cinfo.output_width;
	*heightPtr = cinfo.output_height;
	bytesPerPix = cinfo.output_components;

	lineBuf = cinfo.mem->alloc_sarray ((j_common_ptr) &cinfo, JPOOL_IMAGE, (*widthPtr * bytesPerPix), 1);
	retBuf = malloc (3 * (*widthPtr * *heightPtr));
		
	if (NULL == retBuf) {
		perror (NULL);
		return NULL;
	}
		

	if (3 == bytesPerPix) {
		int lineOffset = (*widthPtr * 3);
		int x;
		int y;
		
		for (y = 0; y < cinfo.output_height; ++y) {
			jpeg_read_scanlines (&cinfo, lineBuf, 1);
				
			for (x = 0; x < lineOffset; ++x) {
				retBuf[(lineOffset * y) + x] = lineBuf[0][x];
				++x;
				retBuf[(lineOffset * y) + x] = lineBuf[0][x];
				++x;
				retBuf[(lineOffset * y) + x] = lineBuf[0][x];
			}
		}
	} else if (1 == bytesPerPix) { 
		unsigned int col;
		int lineOffset = (*widthPtr * 3);
		int lineBufIndex;
		int x ;
		int y;
						
		for (y = 0; y < cinfo.output_height; ++y) {
			jpeg_read_scanlines (&cinfo, lineBuf, 1);
				
			lineBufIndex = 0;
			for (x = 0; x < lineOffset; ++x) {
				col = lineBuf[0][lineBufIndex];
			
				retBuf[(lineOffset * y) + x] = col;
				++x;
				retBuf[(lineOffset * y) + x] = col;
				++x;
				retBuf[(lineOffset * y) + x] = col;
				
				++lineBufIndex;
			}			
		}
	} else {
		fprintf (stderr, "Error: the number of color channels is %d.  This program only handles 1 or 3\n", bytesPerPix);
		return NULL;
	}
	jpeg_finish_decompress (&cinfo);
	jpeg_destroy_decompress (&cinfo);
	fclose (inFile);
			
	return retBuf;
}

XImage *create_image_from_buffer (Display *dis, int screen, u_char *buf, int width, int height) {
	int depth;
	XImage *img = NULL;
	Visual *vis;
	double rRatio;
	double gRatio;
	double bRatio;
	int outIndex = 0;	
	int i;
	int numBufBytes = (3 * (width * height));
		
	depth = DefaultDepth (dis, screen);
	vis = DefaultVisual (dis, screen);

	rRatio = vis->red_mask / 255.0;
	gRatio = vis->green_mask / 255.0;
	bRatio = vis->blue_mask / 255.0;
		
	if (depth >= 24) {
		size_t numNewBufBytes = (4 * (width * height));
		u_int32_t *newBuf = malloc (numNewBufBytes);
	
		for (i = 0; i < numBufBytes; ++i) {
			unsigned int r, g, b;
			r = (buf[i] * rRatio);
			++i;
			g = (buf[i] * gRatio);
			++i;
			b = (buf[i] * bRatio);
					
			r &= vis->red_mask;
			g &= vis->green_mask;
			b &= vis->blue_mask;
			
			newBuf[outIndex] = r | g | b;
			++outIndex;
		}		
		
		img = XCreateImage (dis, 
			CopyFromParent, depth, 
			ZPixmap, 0, 
			(char *) newBuf,
			width, height,
			32, 0
		);
		
	} else if (depth >= 15) {
		size_t numNewBufBytes = (2 * (width * height));
		u_int16_t *newBuf = malloc (numNewBufBytes);
		
		for (i = 0; i < numBufBytes; ++i) {
			unsigned int r, g, b;

			r = (buf[i] * rRatio);
			++i;
			g = (buf[i] * gRatio);
			++i;
			b = (buf[i] * bRatio);
					
			r &= vis->red_mask;
			g &= vis->green_mask;
			b &= vis->blue_mask;
			
			newBuf[outIndex] = r | g | b;
			++outIndex;
		}		
		
		img = XCreateImage (dis,
			CopyFromParent, depth,
			ZPixmap, 0,
			(char *) newBuf,
			width, height,
			16, 0
		);
	} else {
		fprintf (stderr, "This program does not support displays with a depth less than 15.");
		return NULL;				
	}

	XInitImage (img);
	/*Set the client's byte order, so that XPutImage knows what to do with the data.*/
	/*The default in a new X image is the server's format, which may not be what we want.*/
	if ((LSBFirst == get_byte_order ())) {
		img->byte_order = LSBFirst;
	} else {
		img->byte_order = MSBFirst;
	}
	
	/*The bitmap_bit_order doesn't matter with ZPixmap images.*/
	img->bitmap_bit_order = MSBFirst;

	return img;
}

