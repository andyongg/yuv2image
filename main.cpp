#ifdef _MSC_VER
#pragma warning(disable: 4819)
#pragma warning(disable: 4996)
#endif // _MSC_VER

#include <stdio.h>
#include <stdlib.h>
#include "resize_4x.h"
#include "yuv2rgb.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#ifdef _MSC_VER
#define CV_VERSION_NUMBER CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#pragma comment(lib, "opencv_core"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_imgproc"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_highgui"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_video"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_imgcodecs"CV_VERSION_NUMBER"d.lib")
#else
#pragma comment(lib, "opencv_core"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_imgproc"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_highgui"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_video"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_imgcodecs"CV_VERSION_NUMBER".lib")
#endif
#endif // _MSC_VER

using namespace cv;
using namespace std;

typedef enum _yuv_format_t
{
    Y800 = 0, // Simple, single Y plane for monochrome images.
    I420 = 1, // 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes.
    I444 = 2, // 8 bit Y plane followed by 8 bit U and V planes, chroma planes U and V are not sub-sampled.
    NV21 = 3, // 8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling
    NV12 = 4, // As NV12 with U and V reversed in the interleaved plane
} yuv_format_t;

static int read_yuv_file(char *filename, unsigned char *yuvbuf, int yuvbuf_size)
{
    if(filename == NULL || filename[0] == '\0' || yuvbuf == NULL)
    {
        return -1;
    }
    
    FILE *fpi = NULL;
    fpi = fopen(filename, "rb");
    if(fpi == NULL) return -1;
    
    int read_size = fread(yuvbuf, 1, yuvbuf_size, fpi);
    
    if(read_size != yuvbuf_size)
    {
        if(fpi) fclose(fpi);
        return -1;
    }
    else
    {
        if(fpi) fclose(fpi);
        return 0;
    }
}

int main(int argc, char* argv[])
{
    int ret = 0;
    
    if(argc < 6)
    {
	    printf("usage: %s input output format width height stride\n", argv[0]);
        return 0;
    }
    
    int format = atoi(argv[3]);
    int width = atoi(argv[4]);
    int height = atoi(argv[5]);
    int stride = atoi(argv[6]);
    
    if(width < 0 || height < 0 || (width & 0x01) != 0 || (height & 0x01) != 0 || (stride & 0x01) != 0 || stride < width)
    {
        printf("invalid input!\n");
        return -1;
    }
    
    //int stride_half = stride >> 1;
    
    int yuvbuf_size;
    if(format == Y800)
    {
        yuvbuf_size = stride * height * sizeof(unsigned char) * 1;
    }
    else if(format == I420 || format == NV12 || format == NV21)
    {
        yuvbuf_size = stride * height * sizeof(unsigned char) * 3 / 2;
    }
    else
    {
        yuvbuf_size = stride * height * sizeof(unsigned char) * 3;
    }
    
    // malloc memory
    unsigned char *yuvbuf = (unsigned char *)malloc(yuvbuf_size);
    unsigned char *yp = (unsigned char *)malloc(stride * height * sizeof(unsigned char));
    unsigned char *up = (unsigned char *)malloc(stride * height * sizeof(unsigned char));
    unsigned char *vp = (unsigned char *)malloc(stride * height * sizeof(unsigned char));
    unsigned char *rp = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    unsigned char *gp = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    unsigned char *bp = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    if(yuvbuf == NULL || yp == NULL || up == NULL || vp == NULL || rp == NULL || gp == NULL || bp == NULL)
    {
        printf("out of memory!\n");
        if(yuvbuf) free(yuvbuf);
        if(yp) free(yp);
        if(up) free(up);
        if(vp) free(vp);
        if(rp) free(rp);
        if(gp) free(gp);
        if(bp) free(bp);
        return -1;
    }
    
    // read yuv file
    ret = read_yuv_file(argv[1], yuvbuf, yuvbuf_size);
    if(ret != 0)
    {
        printf("read yuv file error!\n");
        if(yuvbuf) free(yuvbuf);
        if(yp) free(yp);
        if(up) free(up);
        if(vp) free(vp);
        if(rp) free(rp);
        if(gp) free(gp);
        if(bp) free(bp);
        return ret;
    }
       
    // convert yuv format to I444
    memcpy(yp, yuvbuf, stride * height * sizeof(unsigned char));
    if(format == Y800)
    {
        memset(up, 128, stride * height * sizeof(unsigned char));
        memset(vp, 128, stride * height * sizeof(unsigned char));
    }
    else if(format == I420)
    {
        unsigned char *src = yuvbuf + stride * height;
        unsigned char *buf_tmp = yuvbuf;
        int uv_height = height / 2;
        int uv_stride = stride / 2;
        
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[y*uv_stride + x];
            }
        }
        
        resize_4x(buf_tmp, up, stride, height);
        
        src = yuvbuf + stride * height + uv_stride * uv_height;
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[y*uv_stride + x];
            }
        }
        
        resize_4x(buf_tmp, vp, stride, height);
    }
    else if(format == NV12)
    {
        unsigned char *src = yuvbuf + stride * height;
        unsigned char *buf_tmp = yuvbuf;
        int uv_height = height / 2;
        int uv_stride = stride / 2;
        
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[(y*uv_stride + x) * 2];
            }
        }
        
        resize_4x(buf_tmp, up, stride, height);
        
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[(y*uv_stride + x) * 2 + 1];
            }
        }
        
        resize_4x(buf_tmp, vp, stride, height);
    }
    else if(format == NV21)
    {
        unsigned char *src = yuvbuf + stride * height;
        unsigned char *buf_tmp = yuvbuf;
        int uv_height = height / 2;
        int uv_stride = stride / 2;
        
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[(y*uv_stride + x) * 2];
            }
        }
        
        resize_4x(buf_tmp, vp, stride, height);
        
        for(int y=0; y<uv_height; y++)
        {
            for(int x=0; x<uv_stride; x++)
            {
                buf_tmp[y*uv_stride + x] = src[(y*uv_stride + x) * 2 + 1];
            }
        }
        
        resize_4x(buf_tmp, up, stride, height);
    }
    else // I444
    {
        memcpy(up, yuvbuf + stride * height, stride * height * sizeof(unsigned char));
        memcpy(vp, yuvbuf + stride * height * 2, stride * height * sizeof(unsigned char));
    }
    
    // convert I444 to rgb
    yuv2rgb(yp, up, vp, rp, gp, bp, width, height, stride);
    
    // image write
    Mat img(cvSize(width, height), CV_8UC3);
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            img.data[(y*width + x) * 3 + 0] = bp[y*width + x];
            img.data[(y*width + x) * 3 + 1] = gp[y*width + x];
            img.data[(y*width + x) * 3 + 2] = rp[y*width + x];
        }
    }
    
    imwrite(argv[2], img);
    
    // release memory
    if(yuvbuf) free(yuvbuf);
    if(yp) free(yp);
    if(up) free(up);
    if(vp) free(vp);
    if(rp) free(rp);
    if(gp) free(gp);
    if(bp) free(bp);
    
    return ret;
}