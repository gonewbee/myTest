/**
 * 使用libpng.so读png文件，命令行参数作为输入文件
 * 编译gcc xx.c -lpng
 */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <png.h>

/* 帮助信息 */
static void usage(FILE *fp, int argc, char** argv) {
    fprintf(fp,
            "Usage: %s [options]\n\n"
            "Version 1.0.1\n"
            "Options:\n"
            "-i | --input   Input image name\n"
            "-h | --help    Print this message\n"
            "-r | --red     Save red channel\n"
            "-g | --green   Save green channel\n"
            "-b | --blue    Save blue channel\n"
            "",
            argv[0]);
}

/** 命令行参数 */
static const char short_options[] = "i:hrgb";
static const struct option
long_options[] = {
    {"input",   required_argument,  NULL, 'i'}, ///<输入图像
    {"help",    no_argument,        NULL, 'h'}, ///帮助
    {"red",     no_argument,        NULL, 'r'}, ///输出红色通道
    {"green",   no_argument,        NULL, 'g'}, ///输出绿色通道
    {"blue",    no_argument,        NULL, 'b'}, ///输出蓝色通道
    {0, 0, 0, 0}
};

void debug_printHex(png_bytep buf, int len) {
    int i = 0;
    for (i=0; i<len; i++) {
        if (0==i%16)
            printf("\n");
        printf("%x ", buf[i]);
    }
    printf("\n");
}

static void PNGCBAPI
pngtest_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_size_t check = 0;
    png_voidp io_ptr;

    io_ptr = png_get_io_ptr(png_ptr);
    //fprintf(stdout, "%s length: %d\n", __func__, length);
    if (NULL != io_ptr) {
        check = fread(data, 1, length, (png_FILE_p)io_ptr);
    }
    if (length != check) {
        fprintf(stderr, "Read Error\n");
    }
}

static void PNGCBAPI
pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_size_t check;
    check = fwrite(data, 1, length, (png_FILE_p)png_get_io_ptr(png_ptr));
    if (check != length) {
        fprintf(stderr, "Write Error\n");
    }
}

/**
 * 保存某一通道
 * 当为RGBA格式时，要保留A通道
 */
static void save_channels(png_structrp png_ptr, png_bytep row_bufi, 
        png_uint_32 num_rows, png_size_t rowbytes, int channel, int channels) {
    int i = 0;
    int index=0;
    if (channel>=channels) {
        fprintf(stderr, "channel not exit\n");
        longjmp(png_jmpbuf(png_ptr), 1);
    }
    //png_bytep row_bufo = (png_bytep)png_malloc(png_ptr, rowbytes);
#if 1
    for (i=0; i<rowbytes; i++) {
        index = i%channels;
        if (4==channels) {
            if ((index!=channel)&&(index!=3))
                row_bufi[i] = 0;
        } else {
            if (index!=channel)
                row_bufi[i] = 0;
        }
    }
#endif
    png_write_rows(png_ptr, (png_bytepp)&row_bufi, num_rows);
}

int main(int argc, char **argv) {
    static png_FILE_p fpin = NULL;
    static png_FILE_p fpout = NULL;
    png_structp read_ptr;
    png_infop read_info_ptr, end_info_ptr;
    png_structp write_ptr;
    png_infop write_info_ptr, write_end_info_ptr;

    png_uint_32 width, height;
    int num_pass = 1, pass = 0;
    int bit_depth, color_type;
    int interlace_type, compression_type, filter_type;
    png_bytep row_buf = NULL;
    png_uint_32 y;
    int idx, c;
    int channel = -1;  //显示那个通道
    int channels = 3;//总通道数
    for (;;) {
        /* 解析命令行参数选项 */
        c = getopt_long(argc, argv, short_options, long_options, &idx);
        if (-1==c)
            break;
        switch (c) {
        case 0:
            break;
        case 'i':
            if (NULL==(fpin=fopen(optarg, "rb"))) {
                fprintf(stderr, "Could not find input file %s\n", optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
            usage(stdout, argc, argv);
            exit(EXIT_SUCCESS);
            break;
        case 'r':
            channel = 0;
            break;
        case 'g':
            channel = 1;
            break;
        case 'b':
            channel = 2;
            break;
        }
    }
    if (NULL==fpin) {
        fprintf(stderr, "Could not find input file\n");
        return 1;
    }
    if (NULL==(fpout=fopen("out.png", "wb"))) {
        fprintf(stderr, "Count not open output file\n");
        return 1;
    }
    read_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (read_ptr&&write_ptr) {
        read_info_ptr = png_create_info_struct(read_ptr);
        end_info_ptr = png_create_info_struct(read_ptr);
        write_info_ptr = png_create_info_struct(write_ptr);
        write_end_info_ptr = png_create_info_struct(write_ptr);
        //printf("Setting jmpbuf for read struct\n");
        if (setjmp(png_jmpbuf(read_ptr))) {
            fprintf(stderr, "setjmp destroy\n");
            png_free(read_ptr, row_buf);
            row_buf = NULL;
            png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
            png_destroy_info_struct(write_ptr, &write_end_info_ptr);
            png_destroy_write_struct(&write_ptr, &write_info_ptr);
            fclose(fpin);
            fclose(fpout);
            return 1;
        }
        if (setjmp(png_jmpbuf(write_ptr))) {
            fprintf(stderr, "setjmp destroy\n");
            png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
            png_destroy_info_struct(write_ptr, &write_end_info_ptr);
            png_destroy_write_struct(&write_ptr, &write_info_ptr);
            fclose(fpin);
            fclose(fpout);
            return 1;
            
        }
        png_set_read_fn(read_ptr, (png_voidp)fpin, pngtest_read_data);
        png_set_write_fn(write_ptr, (png_voidp)fpout, pngtest_write_data, 
                NULL);
        png_read_info(read_ptr, read_info_ptr);
        png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
                &color_type, &interlace_type, &compression_type, &filter_type);
        fprintf(stdout, "png width:%d; height:%d; bit_depth:%d; color_type:%d\n", width, height, bit_depth, color_type);
        png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
                color_type, interlace_type, compression_type, filter_type);
        switch (interlace_type) {
        case PNG_INTERLACE_NONE:
            num_pass = 1;
            break;
        case PNG_INTERLACE_ADAM7:
            num_pass = 7;
            break;
        default:
            png_error(read_ptr, "invalid interlace type");
        }
        fprintf(stdout, "num_pass:%d\n", num_pass);
        switch(color_type) {
            case PNG_COLOR_TYPE_RGB:
                fprintf(stdout, "PNG_COLOR_TYPE_RGB\n");
                channels = 3;
                break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                fprintf(stdout, "PNG_COLOR_TYPE_RGB_ALPHA\n");
                channels = 4;
                break;
            default:
                fprintf(stdout, "other type\n");
        }
        png_write_info(write_ptr, write_info_ptr);
        //fprintf(stdout, "png_get_rowbytes:%d\n", png_get_rowbytes(read_ptr, read_info_ptr));
        row_buf = (png_bytep)png_malloc(read_ptr,
                png_get_rowbytes(read_ptr, read_info_ptr));
        for (pass=0; pass<num_pass; pass++) {
            for (y=0; y<height; y++) {
                png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);
                if (0==y)
                    debug_printHex(row_buf, 16);
                if (-1!=channel)
                    save_channels(write_ptr, row_buf, 1, 
                            png_get_rowbytes(read_ptr, read_info_ptr), channel, channels);
                else
                    png_write_rows(write_ptr, (png_bytepp)&row_buf, 1);
            }
        }
        png_free(read_ptr, row_buf);
        row_buf = NULL;

    }
    fclose(fpin);
    fclose(fpout);
    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
    png_destroy_info_struct(write_ptr, &write_end_info_ptr);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    return 0;
}
