//
// Created by ggauthie on 10/06/2021.
//

#include "resize.h"
#include <stdio.h>
#include "stdlib.h"
#include "math.h"

void resize(unsigned char *src, unsigned char *dest, int *x_rect, int *y_rect, int *width_rect, int *height_rect, int width, int height, int width_final, int height_final)
{
    if(src!=NULL && dest!=NULL && *width_rect!=0 && *height_rect!=0)
    {
        unsigned char box[3**width_rect**height_rect];

        if (((double)*width_rect/(double)width_final) > ((double)*height_rect/(double)height_final))
        {
            double ratio = (double)width_final/(double)*width_rect;

            int width_resize = width_final, height_resize= ratio**height_rect;

            int padding_top = (height_final-height_resize)/2;
            int padding_bottom = (height_final-height_resize) - padding_top;
            unsigned char resizeBox[3*width_resize*height_resize];

            // Crop the image around the box
            for (int y = 0; y < *height_rect; y++)
            {
                for (int x = 0; x < *width_rect; x++)
                {
                    box[3 * (x + y * *width_rect)] = src[3 * (x + *x_rect + (y + *y_rect) * width)];
                    box[3 * (x + y * *width_rect) + 1] = src[3 * (x + *x_rect + (y + *y_rect) * width) + 1];
                    box[3 * (x + y * *width_rect) + 2] = src[3 * (x + *x_rect + (y + *y_rect) * width) + 2];
                }
            }
            // Resize the cropped image with new height and width
            resizeBilinear(box, resizeBox, *width_rect, *height_rect, width_resize, height_resize);
            //Fill the image padding top in black
            for(int x = 0; x < width_resize;x++)
            {
                for(int y = 0; y < padding_top; y++)
                {
                    dest[3 * (x + y * width_resize)] = 0;
                    dest[3 * (x + y * width_resize) + 1] = 0;
                    dest[3 * (x + y * width_resize) + 2] = 0;
                }
            }
            // fill the image destination with the image resize
            for(int x = 0; x < width_resize;x++)
            {
                for(int y = 0; y < height_resize; y++)
                {
                    dest[3 * (x + (padding_top + y) * width_resize)] = resizeBox[3 * (x + y * width_resize)];
                    dest[3 * (x + (padding_top + y) * width_resize) + 1] = resizeBox[3 * (x + y * width_resize) + 1];
                    dest[3 * (x + (padding_top + y) * width_resize) + 2] = resizeBox[3 * (x + y * width_resize) + 2];
                }
            }
            //Fill the image padding bottom in black
            for(int x = 0; x < width_resize;x++)
            {
                for(int y = 0; y < padding_bottom; y++)
                {
                    dest[3 * (x + (padding_top + height_resize + y) * width_resize)] = 0;
                    dest[3 * (x + (padding_top + height_resize + y) * width_resize) + 1] = 0;
                    dest[3 * (x + (padding_top + height_resize + y) * width_resize) + 2] = 0;
                }
            }
        }
        else
        {
            double ratio = (double)height_final/(double)*height_rect;
            int width_resize = *width_rect*ratio, height_resize= height_final;
            int padding_left = (width_final-width_resize)/2;
            int padding_right = (width_final-width_resize) - padding_left;
            unsigned char resizeBox[3*width_resize*height_resize];
            // Crop the image around the box
            for (int y = 0; y < *height_rect; y++)
            {
                for (int x = 0; x < *width_rect; x++)
                {
                    box[3 * (x + y * *width_rect)] = src[3 * (x + *x_rect + (y + *y_rect) * width)];
                    box[3 * (x + y * *width_rect) + 1] = src[3 * (x + *x_rect + (y + *y_rect) * width) + 1];
                    box[3 * (x + y * *width_rect) + 2] = src[3 * (x + *x_rect + (y + *y_rect) * width) + 2];
                }
            }
            // Resize the cropped image with new height and width
            resizeBilinear(box, resizeBox, *width_rect, *height_rect, width_resize, height_resize);
            //Fill the image padding top in black
            for(int x = 0; x < padding_left;x++)
            {
                for(int y = 0; y < height_resize; y++)
                {
                    dest[3 * (x + y * width_final)] = 0;
                    dest[3 * (x + y * width_final) + 1] = 0;
                    dest[3 * (x + y * width_final) + 2] = 0;
                }
            }
            // fill the image destination with the image resize
            for(int x = 0; x < width_resize;x++)
            {
                for(int y = 0; y < height_resize; y++)
                {
                    dest[3 * (x + padding_left + y * width_final)] = resizeBox[3 * (x + y * width_resize)];
                    dest[3 * (x + padding_left + y * width_final)+ 1] = resizeBox[3 * (x + y * width_resize)+ 1];
                    dest[3 * (x + padding_left + y * width_final)+ 2] = resizeBox[3 * (x + y * width_resize)+ 2];
                }
            }
            //Fill the image padding bottom in black
            for(int x = 0; x < padding_right;x++)
            {
                for(int y = 0; y < height_resize; y++)
                {
                    dest[3 * (x + padding_left + width_resize + y * width_final)] = 0;
                    dest[3 * (x + padding_left + width_resize + y * width_final) + 1] = 0;
                    dest[3 * (x + padding_left + width_resize + y * width_final) + 2] = 0;
                }
            }
        }
    }
    else
    {
        //printf("error in resize image\n");
    }
}
void resizeWithPadding(unsigned char *src, unsigned char *dest, int *x_rect, int *y_rect, int *width_rect, int *height_rect, int width, int height, int width_final, int height_final)
{

}

void resizeBilinear(unsigned char *src, unsigned char *dest, int w, int h, int w2, int h2)
{
    int x, y, index;
    float x_ratio = (((float) (w - 1)) / ((float) w2));
    float y_ratio = (((float) (h - 1)) / ((float) h2));

    float x_diff, y_diff;
    unsigned char blue, red, green;
    int offset = 0;
    for (int i = 0; i < h2; i++)
    {
        for (int j = 0; j < w2; j++)
        {
            x = (x_ratio * j);
            y = (y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            index = (y * w + x);

            //Red component
            dest[3*(j+i*w2)] = (unsigned char) floor((src[3*index]) * (1 - x_diff) * (1 - y_diff) +
                                                     (src[3*(index + 1)]) * (x_diff) * (1 - y_diff) +
                                                     (src[3*(index + w)]) * (y_diff) * (1 - x_diff) +
                                                     (src[3*(index + w + 1)]) * (x_diff * y_diff));

            //Green component
            dest[3*(j+i*w2)+1] = (unsigned char) floor((src[3*index +1]) * (1 - x_diff) * (1 - y_diff) +
                                                     (src[3*(index + 1)+ 1]) * (x_diff) * (1 - y_diff) +
                                                     (src[3*(index + w)+1]) * (y_diff) * (1 - x_diff) +
                                                     (src[3*(index + w + 1)+1]) * (x_diff * y_diff));
            // Blue component
            dest[3*(j+i*w2)+2] = (unsigned char) floor((src[3*index+2]) * (1 - x_diff) * (1 - y_diff) +
                                                     (src[3*(index + 1)+2]) * (x_diff) * (1 - y_diff) +
                                                     (src[3*(index + w)+2]) * (y_diff) * (1 - x_diff) +
                                                     (src[3*(index + w + 1)+2]) * (x_diff * y_diff));

        }
    }
}

