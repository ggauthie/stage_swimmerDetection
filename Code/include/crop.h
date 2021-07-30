//
// Created by ggauthie on 02/06/2021.
//

#ifndef SWIMMERDETECTION_C_CROP_H
#define SWIMMERDETECTION_C_CROP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void crop(unsigned char* src, unsigned char* dest, int height, int width, int x_rect, int y_rect, int width_rect, int height_rect);

void crop_y(unsigned char* src, unsigned char* dest, int width, int height, int y_left);

#endif //SWIMMERDETECTION_C_CROP_H
