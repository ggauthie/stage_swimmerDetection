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
    static long sumVar[3*480*640];
    static long sumVarSq[3*480*640];
    static int nb_frames =1;


void swimmerAlgoDynamic(int width, int height,unsigned char *src, unsigned char *dest, Rectangle *box)
{
	swimmerAlgoDynamic_cpp(src, dest, width, height, box);
}

void medianFilter(int width, int height, int nb_channels, unsigned char *src, unsigned char *dest)
{
    medianFilter_cpp(src, dest, width, height, nb_channels);
}

void cvtColorHSV(int width, int height, unsigned char *src, unsigned char *dest)
{
    cvtColorHSV_cpp(src, dest, width,height);
}

void boxConstruction(int width, int height,unsigned char* src, Rectangle *bb)
{
    boxConstruction_cpp(src, width, height, bb);
}

void cvtToWB(int width, int height, unsigned char* src, unsigned char* dest)
{
	cvtToWB_cpp(src,dest,width,height);
}

void normRGB(int width, int height, unsigned char* src, unsigned char *dest)
{
    for(int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            dest[j+i*width]=sqrt(pow(src[3*(j+i*width)],2) + pow(src[3*(j+i*width) + 1],2) + pow(src[3*(j+i*width) +2],2));
        }
    }
}

void variance( int width, int height, int nb_frames, unsigned char *src, unsigned char *dest)
{
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

void variance_iter(int width, int height, unsigned char *src, unsigned char *dest)
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
void threshold(int width, int height, int thresh, unsigned char *src, unsigned char *dest)
{
    threshold_cpp(src, dest, width, height, thresh);
}
void opening(int width, int height, int k, unsigned char *src, unsigned char *dest)
{
    opening_cpp(src, dest, width, height,k);
}

void closing(int width, int height, int k, unsigned char *src, unsigned char *dest)
{
    closing_cpp(src, dest, width, height, k);
}

void generateMaskSurface(int width, int height, unsigned char* src)
{
    generateMaskSurface_cpp(src,width,height);
}

void applyMaskSurface(int width, int height, unsigned char *src, unsigned char *dest)
{
    applyMaskSurface_cpp(src, dest, width, height);
}

void deleteLight(int width, int height, unsigned char *src, unsigned char *dest)
{
    deleteLight_cpp(src, dest, width, height);
}

void bitwise_and(unsigned char *src, unsigned char * mask, unsigned char *dest, int width, int height, int nb_channels)
{
    bitwise_and_cpp(src,mask, dest,width,height, nb_channels);
}

void segmentationHSV(int width, int height, unsigned char *src, unsigned char *dest)
{
    segmentationHSV_cpp(src, dest, width, height);
}

void segmentationYUV(int width, int height, unsigned char *src, unsigned char *dest)
{
     segmentationYUV_cpp(src, dest, width, height);
}

void cvtColorRGBtoYUV(int width, int height, unsigned char *src, unsigned char *dest)
{
    cvtColorRGBtoYUV_cpp(src, dest, width, height);
}

void mergeBox(Rectangle *box1, Rectangle *box2, Rectangle *boxMerged)
{
    mergeBox_cpp(box1, box2, boxMerged);
}

void drawRectangle(int width, int height, unsigned char *src, Rectangle *bb, unsigned char *dest)
{
    drawRectangle_cpp(src, dest, bb, width, height);
}

void iou(Rectangle *r1, Rectangle* r2, float* iou)
{
	(*iou)= iou_cpp(r1, r2);
}

float iou_1(Rectangle *r1, Rectangle* r2)
{
	return iou_cpp(r1, r2);
}


/*void readJson()
{
    Image_instance->readJson();
}*/

#ifdef __cplusplus
}
#endif

