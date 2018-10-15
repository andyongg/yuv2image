#include <stdio.h>
#include "resize_4x.h"

// x4 bilinear upscale
void resize_4x(unsigned char *src, unsigned char *dst, int width, int height)
{
    int width_half = width >> 1;
    int height_half = height >> 1;

    unsigned char *src_line0 = src;
    unsigned char *src_line1 = src_line0 + width_half;
    unsigned char *dst_line0 = dst;
    unsigned char *dst_line1;
    
    // first line
    *(dst_line0 ++) = src_line0[0];
    for(int x = 1; x < width_half; x++)
    {
        *(dst_line0 ++) = (src_line0[x-1] * 3 + src_line0[x] * 1) >> 2;
        *(dst_line0 ++) = (src_line0[x-1] * 1 + src_line0[x] * 3) >> 2;
    }
    
    *(dst_line0 ++) = src_line0[width_half-1];
    
    // middle bilinear interpolate
    dst_line1 = dst_line0 + width;
    for(int y = 1; y < height_half; y++)
    {
        *(dst_line0 ++) = (src_line0[0] * 3 + src_line1[1] * 1) >> 2;
        *(dst_line1 ++) = (src_line0[0] * 1 + src_line1[1] * 3) >> 2;
        
        for(int x = 1; x < width_half; x++)
        {
            *(dst_line0 ++) = (src_line0[x-1] * 9 + src_line0[x] * 3 + src_line1[x-1] * 3 + src_line1[x] * 1) >> 4;
            *(dst_line0 ++) = (src_line0[x-1] * 3 + src_line0[x] * 9 + src_line1[x-1] * 1 + src_line1[x] * 3) >> 4;
            *(dst_line1 ++) = (src_line0[x-1] * 3 + src_line0[x] * 1 + src_line1[x-1] * 9 + src_line1[x] * 3) >> 4;
            *(dst_line1 ++) = (src_line0[x-1] * 1 + src_line0[x] * 3 + src_line1[x-1] * 3 + src_line1[x] * 9) >> 4;
        }
        
        *(dst_line0 ++) = (src_line0[width_half-1] * 3 + src_line1[width_half-1] * 1) >> 2;
        *(dst_line1 ++) = (src_line0[width_half-1] * 1 + src_line1[width_half-1] * 3) >> 2;
        
        dst_line0 += width;
        dst_line1 += width;
        src_line0 += width_half;
        src_line1 += width_half;
    }
    
    // last line
    *(dst_line0 ++) = src_line0[0];
    for(int x = 1; x < width_half; x++)
    {
        *(dst_line0 ++) = (src_line0[x-1] * 3 + src_line0[x] * 1) >> 2;
        *(dst_line0 ++) = (src_line0[x-1] * 1 + src_line0[x] * 3) >> 2;
    }
    
    *(dst_line0 ++) = src_line0[width_half-1];
}
