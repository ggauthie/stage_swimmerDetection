//
// Created by ggauthie on 10/06/2021.
//

#include "resize.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

void resize(unsigned char *src, unsigned char *dest, Rectangle *box, int width, int height, int width_final, int height_final)
{
    if(src!=NULL && dest!=NULL && box->w!=0 && box->h!=0)
    {
        unsigned char img[3*box->w*box->h];

        if (((double)box->w/(double)width_final) > ((double)box->h/(double)height_final))
        {
            double ratio = (double)width_final/(double)box->w;

            int width_resize = width_final, height_resize= ratio*box->h;

            int padding_top = (height_final-height_resize)/2;
            int padding_bottom = (height_final-height_resize) - padding_top;
            unsigned char resizeBox[3*width_resize*height_resize];

            // Crop the image around the box
            for (int y = 0; y < box->h; y++)
            {
            	memcpy(img + 3*y*box->w, src + 3 *(box->x +(y+box->y)*width), 3*(box->w));
            }
            // Resize the cropped image with new height and width
            resizeBilinear(img, resizeBox, box->w, box->h, width_resize, height_resize);

            //Fill the image padding top in black
            memset(dest,0, 3*width_final*padding_top);

            // fill the image destination with the image resize
            memcpy(dest + 3 *padding_top*width_final, resizeBox, 3*width_final*height_resize);

            //Fill the image padding bottom in black
            memset(dest + 3 *(padding_top + height_resize)* width_final,0, 3*width_final*padding_bottom);

        }
        else
        {
            double ratio = (double)height_final/(double)box->h;
            int width_resize = box->w*ratio, height_resize= height_final;
            int padding_left = (width_final-width_resize)/2;
            int padding_right = (width_final-width_resize) - padding_left;
            unsigned char resizeBox[3*width_resize*height_resize];
            // Crop the image around the img
            for (int y = 0; y < box->h; y++)
            {
            	memcpy(img + 3*y*box->w, src + 3 *(box->x +(y+box->y)*width), 3*(box->w));


            }
            // Resize the cropped image with new height and width
            resizeBilinear(img, resizeBox, box->w, box->h, width_resize, height_resize);
            //Fill the image padding left in black
            for(int y = 0; y < height_final; y++)
            {
            	memset(dest + 3*y*width_final, 0, 3*padding_left);
            }

            // fill the image destination with the image resize
            for(int y = 0; y < height_final; y++)
            {
            	memcpy(dest + 3*y*width_final + 3*padding_left, resizeBox + y*3*width_resize, 3*width_resize);
            }

            //Fill the image padding rightin black
            for(int y = 0; y < height_final; y++)
            {
            	memset(dest + 3*y*width_final + 3*(padding_left +width_resize), 0, 3*padding_right);
            }

        }
    }
    else
    {
        //printf("error in resize image\n");
    }
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

