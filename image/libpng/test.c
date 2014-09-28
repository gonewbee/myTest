/**
 * 使用libpng.so读png文件，命令行参数作为输入文件
 * 编译gcc xx.c -lpng
 */
#include <stdio.h>
#include <png.h>

static void PNGCBAPI
pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_size_t check = 0;
    png_voidp io_ptr;

    io_ptr = png_get_io_ptr(png_ptr);
    fprintf(stdout, "%s length: %d\n", __func__, length);
    if (NULL != io_ptr) {
        check = fread(data, 1, length, (png_FILE_p)io_ptr);
    }
    if (length != check) {
        fprintf(stderr, "Read Error\n");
    }
}

int main(int argc, char **argv) {
    static png_FILE_p fpin;
    png_infop read_info_ptr, end_info_ptr;
    png_structp read_ptr;

    png_uint_32 width, height;
    int bit_depth, color_type;

    if (2!=argc) {
        fprintf(stderr, "not input file\n");
        return 1;
    }
    if (NULL==(fpin=fopen(argv[1], "rb"))) {
        fprintf(stderr, "Could not find input file %s\n", argv[1]);
        return 1;
    }
    read_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (read_ptr) {
        read_info_ptr = png_create_info_struct(read_ptr);
        end_info_ptr = png_create_info_struct(read_ptr);
        printf("Setting jmpbuf for read struct\n");
        if (setjmp(png_jmpbuf(read_ptr))) {
            fprintf(stderr, "setjmp destroy\n");
            png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
            return 1;
        }
        png_set_read_fn(read_ptr, (png_voidp)fpin, pngtest_read_data);
        png_read_info(read_ptr, read_info_ptr);
        png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
                &color_type, 0, 0, 0);
        fprintf(stdout, "png width:%d; height:%d; bit_depth:%d; color_type:%d\n", width, height, bit_depth, color_type);
        switch(color_type) {
            case PNG_COLOR_TYPE_RGB:
                fprintf(stdout, "PNG_COLOR_TYPE_RGB\n");
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                fprintf(stdout, "PNG_COLOR_TYPE_RGB_ALPHA\n");
                break;
            default:
                fprintf(stdout, "other type\n");
        }
        fclose(fpin);
        png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
        return 0;
    }

}
