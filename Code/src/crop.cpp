//
// Created by ggauthie on 02/06/2021.
//

#include "crop.h"

void crop(unsigned char* src, unsigned char* dest, int height, int width, int x_rect, int y_rect, int width_rect, int height_rect)
{
    if(src && dest)
    {
        for(int y=0; y<height_rect; y++)
        {
            for(int x=0;x<width_rect; x++)
            {
                dest[3*(x+y*width_rect)] = src[3*(x+x_rect +(y+y_rect)*width)];
                dest[3*(x+y*width_rect) + 1] = src[3*(x+x_rect +(y+y_rect)*width)+1];
                dest[3*(x+y*width_rect) + 2] = src[3*(x+x_rect +(y+y_rect)*width)+2];
            }
        }
    }
}

void crop_y(unsigned char* src, unsigned char* dest, int width,  int height, int y_left)
{
	memcpy(dest,src + 3*y_left*width,3*(height-y_left)*width);
}

void cropWithPadding(unsigned char* src, unsigned char* dest, int height, int width, int x_rect, int y_rect, int width_rect, int height_rect, int padding)
{
    if(x_rect - padding < 0)
        x_rect = 0;
    else
        x_rect = x_rect - padding;

    if(y_rect - padding < 0)
        y_rect = 0;
    else
        y_rect = y_rect - padding;

    if(x_rect + width_rect + padding > width)
        width_rect = width;
    else
        width_rect = x_rect + width_rect + padding;

    if(y_rect + height_rect + padding > height)
        height_rect = height;
    else
        height_rect = y_rect + height_rect + padding;

    crop(src, dest, height, width, x_rect, y_rect, width_rect, height_rect);

}
