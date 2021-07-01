//
// Created by ggauthie on 10/06/2021.
//

#ifndef SWIMMERDETECTION_C_RESIZE_H
#define SWIMMERDETECTION_C_RESIZE_H

#include "crop.h"
#include "rectangle.h"

void resize(unsigned char *src, unsigned char *dest, Rectangle * box, int width, int height, int width_final, int height_final);
void resizeBilinear(unsigned char *src, unsigned char *dest, int w, int h, int w2, int h2);
#endif //SWIMMERDETECTION_C_RESIZE_H
