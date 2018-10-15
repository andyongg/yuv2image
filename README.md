
yuv2image is an open source tool for converting YUV format file to image file(i.e. jpg/bmp/png).

A precompiled Windows binary of yuv2image can be downloaded from [the downloads page](http://www.xiamiworks.com/downloads/).

## Supported YUV pixel formats

Currently yuv2image supports the following YUV pixel formats:
<table class='smalltext altrows' markdown=1>
<thead>
<tr>
<th >Format
</th>
<th >Value
</th>
<th >Description
</th>
</tr>
</thead>
<tbody >
<tr>
<td>Y800
</td>
<td>0
</td>
<td>Simple, single Y plane for monochrome images.
</td>
</tr>
<tr>
<td>I420
</td>
<td>1
</td>
<td>8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes.
</td>
</tr>
<tr>
<td>I444
</td>
<td>2
</td>
<td>8 bit Y plane followed by 8 bit U and V planes, chroma planes U and V are not sub-sampled.
</td>
</tr>
<tr>
<td>NV21
</td>
<td>3
</td>
<td>8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling.
</td>
</tr>
<tr>
<td>NV12
</td>
<td>4
</td>
<td>As NV12 with U and V reversed in the interleaved plane.
</td>
</tr>
</tbody>
</table>

## How do I convert YUV format file to image using yuv2image?

Invoke yuv2image from command line, usage:

    yuv2image <YUV file> <image file> <format {0,1,2,3,4}> <width> <height> <stride>
e.g. to convert a nv21 YUV pixel format file to jpg:

    $ ./yuv2image 4032x3024_nv21.yuv output.jpg 3 4032 3024 4032


