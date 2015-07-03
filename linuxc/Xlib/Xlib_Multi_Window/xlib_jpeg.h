#ifndef _XLIB_JPEG_H
#define _XLIB_JPEG_H
/**
 * @brief 从jpeg图像中获取图像数据
 */
u_char *decode_jpeg (char *filename, int *widthPtr, int *heightPtr);

/**
 * @brief 使用从jpeg图像中获取的图像数据建立XImage
 */
XImage *create_image_from_buffer (Display *dis, int screen, u_char *buf, int width, int height);
#endif
