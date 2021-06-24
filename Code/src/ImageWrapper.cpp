//
// Created by ggauthie on 31/05/2021.
//

// MyObjWrapper.cpp
#include "../include/Image.h"
#include "../include/ImageWrapper.h"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;

/* use C name mangling */
#ifdef __cplusplus
extern "C" {
#endif
    static int id_frame = 0;
    static long sumVar[3*360*640];
    static long sumVarSq[3*360*640];
    static int nb_frames =1;
void cannyMethod(unsigned char *src, unsigned char* dest, int width, int height)
{
    cannyMethod_cpp(src,dest,width,height);
}

void otsuMethod(unsigned char *src, unsigned char* dest, int width, int height)
{
    otsuMethod_cpp(src,dest,width,height);
}
/*
void swimmerAlgo(unsigned char *src, unsigned char* dest, int width, int height, int *x_rect, int *y_rect, int *width_rect, int *height_rect)
{
    Image_instance->swimmerAlgo(src,dest,width,height, x_rect, y_rect, width_rect, height_rect);
}*/

void swimmerAlgoDynamic(unsigned char *src, unsigned char *dest, int width, int height)
{
	swimmerAlgoDynamic_cpp(src, dest,width, height);
}

void medianFilter(unsigned char *src, unsigned char *dest, int width, int height, int nb_channels)
{
    medianFilter_cpp(src, dest, width, height, nb_channels);
}

void cvtColorHSV(unsigned char *src, unsigned char *dest, int width, int height)
{
    cvtColorHSV_cpp(src, dest, width,height);
}

void maskCreationSimple(unsigned char *src, unsigned char *dest, int width, int height)
{
	maskCreationSimple_cpp(src, dest, width, height);
}

void maskCreation(unsigned char *src, unsigned char* mask, unsigned char *dest, int width, int height)
{
    maskCreation_cpp(src, mask, dest, width, height);
}

void boxConstruction(unsigned char* src, int width, int height, int *x_rect,int *y_rect, int *width_rect, int *height_rect)
{
    boxConstruction_cpp(src, width, height, x_rect, y_rect, width_rect, height_rect);
}

void cvtToWB(unsigned char* src, unsigned char* dest, int width, int height)
{
	cvtToWB_cpp(src,dest,width,height);
}

void saliencySpectralRes(unsigned char* src, unsigned char* dest, int width, int height)
{
    saliencySpectralRes_cpp(src,dest,width,height);
}

void saliencyFineGrained(unsigned char* src, unsigned char* dest, int width, int height)
{
    saliencyFineGrained_cpp(src,dest,width,height);
}

void surfaceDetection(unsigned char* src, unsigned char* dest,int width, int height) {
	unsigned char norm[height*width];
	normRGB(src, norm,width, height);
	surfaceDetection_cpp(norm, src, dest, width,height);
}

void normRGB(unsigned char* src, unsigned char *dest, int width, int height)
{
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            dest[j+i*width]=sqrt(pow(src[3*(j+i*width)],2) + pow(src[3*(j+i*width) + 1],2) + pow(src[3*(j+i*width) +2],2));
        }
    }
}

void variance(unsigned char *src, unsigned char *dest, int width, int height, int nb_frames)
{
    //variance_cpp(src, dest, width, height, nb_frames);
    for(int j=0; j < height; j++)
    {
        for(int i =0; i< width; i++)
        {
            for(int k =0; k< 3; k++)
            {
                long sum = 0;
                long sumSq = 0;
                for(int l=0; l< nb_frames; l++)
                {
                    sum= sum + src[3*(i + j*width)+ k +l*(height*width*3)];
                    sumSq = sumSq + (int)pow(src[3*(i + j*width)+ k +l*(height*width*3)],2);
                }
                dest[3*(i + j*width) + k] = (unsigned char)((sumSq- pow(sum,2)/nb_frames) /(nb_frames-1));
            }
        }
    }
}

void variance_iter(unsigned char *src, unsigned char *dest, int width, int height)
{
    //variance_cpp(src, dest, width, height, nb_frames);
    for(int j=0; j < height; j++)
    {
        for(int i =0; i< width; i++)
        {
            for(int k =0; k< 3; k++)
            {
                sumVar[3*(i + j*width)+ k]= sumVar[3*(i + j*width)+ k] + src[3*(i + j*width)+ k];
                sumVarSq[3*(i + j*width)+ k] = sumVarSq[3*(i + j*width)+ k] + (int)pow(src[3*(i + j*width)+ k ],2);
                dest[3*(i + j*width) + k] = (unsigned char)((sumVarSq[3*(i + j*width)+ k]- pow(sumVar[3*(i + j*width)+ k],2)/nb_frames) /(nb_frames-1));
            }
        }
    }
    nb_frames++;
}
void threshold(unsigned char *src, unsigned char *dest, int width, int height, int thresh)
{
    threshold_cpp(src, dest, width, height, thresh);
}
void opening(unsigned char *src, unsigned char *dest, int width, int height, int k)
{
    opening_cpp(src, dest, width, height,k);
}

void closing(unsigned char *src, unsigned char *dest, int width, int height, int k)
{
    closing_cpp(src, dest, width, height, k);
}
/*void readJson()
{
    Image_instance->readJson();
}*/

#ifdef __cplusplus
}
#endif

