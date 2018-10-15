#ifndef _YUV2RGB_H_
#define _YUV2RGB_H_

void yuv2rgb(unsigned char *yp, unsigned char *up, unsigned char *vp, unsigned char *rp, unsigned char *gp,\
             unsigned char *bp, int width, int height, int stride);

#endif // _YUV2RGB_H_