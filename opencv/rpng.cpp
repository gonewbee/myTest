/**
 * 将文件隐写到png图像的第四通道，前四个像素点为文件长度
 * 1. 读取png图像imread时使用CV_LOAD_IMAGE_UNCHANGED参数，通道数为4
 */
#include <cv.h>
#include <highgui.h>

#ifdef _FPYTHON
#ifdef __cplusplus
extern "C"{
#endif
#endif
using namespace cv;

#ifdef _DEBUG_
int savejar2png(const char* ijar, const char* iImageName, const char *outputPng);
int getfrompng(const char* ifile, const char* outputFile);

int main(int argc, char **argv) {
    savejar2png("test.jar", "test.png", "cvout.png");
    getfrompng("cvout.png", "frompng.jar");
    return 0;
}
#endif

/**
 * 将ijar中的数据隐写到png的第四通道，前四像素点保存的是ijar文
 * 件的长度
 * @param ijar in要隐藏的文件
 * @param iImageName in隐藏文件的载体
 * @param outputPng out输出结果
 * @return 0 成功 -1失败
 */
int savejar2png(const char* ijar, const char* iImageName, const char *outputPng) {
    int rows, cols;
    Mat img = imread(iImageName, CV_LOAD_IMAGE_UNCHANGED);
//    Mat img = imread(iImageName);//使用默认参数，读取png图像通道数为3,无透明通道
    if (!img.data) {
        printf("No image data\n");
        return -1;
    }
    Mat imgOut = img.clone();
    FILE *fin = fopen(ijar, "r+");
    uchar udata = 0;
    int ret = 0;
    rows = img.rows;
    cols = img.cols;
    printf("channels:%d, rows:%d, cols:%d\n", img.channels(), rows, cols);
    //imshow("png", img); //显示时仍然未显示出透明通道
#if 1
    int fileLen=0, index=4;
    int pngLen = img.rows*img.cols;
    uchar *data = imgOut.ptr<uchar>(0);
    while ((ret=fread(&udata, 1, 1, fin))==1) {
        fileLen++;
        if (index>pngLen) {
            printf("png file small\n");
            fclose(fin);
            return -1;
        }
        data[index*4+3] = udata;
        index++;
    }
    printf("fileLen:%d\n", fileLen);
    data[3] = (fileLen>>24)&0xff;
    data[7] = (fileLen>>16)&0xff;
    data[11] = (fileLen>>8)&0xff;
    data[15] = fileLen&0xff;
#else
    int i=0, j=0;
    for (i=0; i<rows; i++) {
        for (j=0; j<cols; j++) {
            ret = fread(&udata, 1, 1, fin);
            Vec4b &pngPoint = imgOut.at<Vec4b>(i, j);
            if (ret==1)
                pngPoint[3] = udata;
        }
    }
#endif
    imwrite(outputPng, imgOut);
    fclose(fin);
    return 0;
}

/**
 * 将png图片的第四通道的数据取出，前四个像素点保存的是文件长度
 * @param iFile in包含目标文件的图像
 * @param outputFile out输出结果
 * @return 0 成功 -1失败
 */
int getfrompng(const char* iFile, const char *outputFile) {
    int rows, cols;
    Mat img = imread(iFile, CV_LOAD_IMAGE_UNCHANGED);
    if (!img.data) {
        printf("No image data\n");
        return -1;
    }
    Vec4b pngPoint;
    uchar *data = img.ptr<uchar>(0);
    int fileLen = (data[3]<<24)+(data[7]<<16)+(data[11]<<8)+data[15];
    printf("fileLen:%d\n", fileLen);
    int pngLen = img.rows*img.cols;
    if ((fileLen+4)>pngLen) {
        printf("fileLen too large\n");
        return -1;
    }
    FILE *fout = fopen(outputFile, "w+");
    int index=0;
    for (index=0; index<fileLen; index++) {
        fwrite(&data[4*index+19], 1, 1, fout);  //跳过前四个像素点
    }
    fclose(fout);
    return 0;
}

#ifdef _FPYTHON
#ifdef __cplusplus
}
#endif
#endif

