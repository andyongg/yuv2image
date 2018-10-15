#include <stdio.h>
#include "yuv2rgb.h"

#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MIN2(a,b) ((a)>(b)?(b):(a))
#define CLIP3(a,low,high) MIN2(MAX2(a,low),high)

void yuv2rgb(unsigned char *yp, unsigned char *up, unsigned char *vp, unsigned char *rp, unsigned char *gp,\
             unsigned char *bp, int width, int height, int stride)
{
    /*
    R'= Y' + 0.000*U' + 1.403*V'
    G'= Y' - 0.344*U' - 0.714*V'
    B'= Y' + 1.773*U' + 0.000*V'
    */
    float mat[9] = {1.0f, 0.0f, 1.403f, 1.0f, - 0.344f, - 0.714f, 1.0f, 1.773f, 0.0f};
    int uv_offset = 128;
    
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            int lum = yp[y*stride + x];
            int u = up[y*stride + x];
            int v = vp[y*stride + x];
            
            int r = (int)(mat[0] * lum + mat[1] * (u - uv_offset) + mat[2] * (v - uv_offset));
            int g = (int)(mat[3] * lum + mat[4] * (u - uv_offset) + mat[5] * (v - uv_offset));
            int b = (int)(mat[6] * lum + mat[7] * (u - uv_offset) + mat[8] * (v - uv_offset));
            
            rp[y*width + x] = CLIP3(r, 0, 255);
            gp[y*width + x] = CLIP3(g, 0, 255);
            bp[y*width + x] = CLIP3(b, 0, 255);
        }
    }
}
