//
// Created by ggauthie on 06/07/2021.
//

#ifndef SWIMMERDETECTION_C_VOTESYSTEM_H
#define SWIMMERDETECTION_C_VOTESYSTEM_H

#include "rectangle.h"
#include "ImageWrapper.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

void poolingDistance(Rectangle *box1, Rectangle *box2, Rectangle *box3, Rectangle *boxFinal);

void poolingIoU(Rectangle *box1, Rectangle *box2, Rectangle *box3, Rectangle *boxFinal);

#endif //SWIMMERDETECTION_C_VOTESYSTEM_H
