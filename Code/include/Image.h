//
// Created by ggauthie on 31/05/2021.
//

#ifndef SWIMMERDETECTION_C_IMAGE_H
#define SWIMMERDETECTION_C_IMAGE_H

// MyObj.h
#pragma once
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/saliency.hpp>

#include "xtensor/xtensor.hpp"
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xadapt.hpp"
#include "xtensor-blas/xlinalg.hpp"

using namespace cv;
using namespace std;


/**
* Function used to detect the swimmer in a frame with the CannyEdge method.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels of the image with the bounding box around the swimmer
*
*/
void cannyMethod_cpp(unsigned char *src, unsigned char* dest, int width, int height);

/**
* Function used to detect the swimmer in a frame with the Otsu method.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels of the image processed
*
*/
void otsuMethod_cpp(unsigned char *src, unsigned char* dest, int width, int height);
//void swimmerAlgo(unsigned char *src, unsigned char* dest, int width, int height, int *x_rect, int *y_rect, int *width_rect, int *height_rect);

/**
* Function used to detect the swimmer in a frame using median blur filter, threshold and color filter on HSV components
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels of the image with the bounding box
*
*/
void swimmerAlgoDynamic_cpp(unsigned char *pixels, unsigned char* mask, unsigned char* dest, int width, int height, int *x_rect, int *y_rect, int *width_rect, int *height_rect);

/**
* Median Blur Filter
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels of the image processed
* @param nb_channels
 *      3 if this is an HSV or RGB image or 1 if this is a WB image
*
*/
void medianFilter_cpp(unsigned char *src, unsigned char *dest, int width, int height, int nb_channels);

/**
* Function used to convert the format of the image : RGB to HSV format
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format) of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels(HSV) of the image processed
*
*/
void cvtColorHSV_cpp(unsigned char *src, unsigned char *dest, int width, int height);

/**
* Function used to create a mask highlighting the swimmer in the image :
* Process :
* - Filter the 3 components H,S,V to create a mask
* - Association of the mask created with the mask of the detection water surface
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format) of the image to process
* @param mask
*       input : the array of size width*height with all the values of pixels of the detection surface mask
* @param dest
*       output : the array of size width*height with all the values of pixels of the final mask created
*
*/
void maskCreationSimple_cpp(unsigned char *src, unsigned char *dest, int width, int height);
void maskCreation_cpp(unsigned char *src, unsigned char* mask, unsigned char *dest, int width, int height);

/**
* Function used to compute the variance on each component of each pixel on all frames
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param nb_frames
 *      the number of frames stored in the array
* @param src
*       the array of size 3*width*height*nb_frames with all the values of pixels(RGB format) of the different frames ( format (1/{RGB RGB RGB..} 2/{RGB RGB ..})
* @param dest
*       output : the array of size 3*width*height with all the values of variance.

*
*/
void variance_cpp(unsigned char *src, unsigned char *dest, int width, int height, int nb_frames);

/**
* Function used to compute the variance in iterative way. Static arrays used in this method :
 * - sumVar[3*height*width] to store the sum of all the components of the pixels
 * - sumVarSq[3*height*width] to store the sum of te square of all the components of the pixels
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format)
* @param dest
*       output : the array of size 3*width*height with all the values of variance.
*
*/
void threshold_cpp(unsigned char *src, unsigned char *dest, int width, int height, int thresh);

/**
* Function which applies an opening operation on an image. The structuring element is a square of size (2*k +1)
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param k
*        the size of the kernel of the structuring element
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format)
* @param dest
*       output : the array of size 3*width*height with all the values of pixels of the image
*
*/
void opening_cpp(unsigned char *src, unsigned char *dest, int width, int height, int k);

/**
* Function which applies a closing operation on an image. The structuring element is a square of size (2*k +1)
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param k
*        the size of the kernel of the structuring element
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format)
* @param dest
*       output : the array of size 3*width*height with all the values of pixels of the image
*
*/
void closing_cpp(unsigned char *src, unsigned char *dest, int width, int height, int k);

/**
* Function which determines the bounding box with the mask in input using findContours in OpenCv
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the mask
* @param x_rect
*       output : the coordinate x of the top left point of the bounding box
* @param y_rect
*       output : the coordinate y of the top left point of the bounding box
* @param width_rect
*       output : the width of the bounding box
* @param height_rect
*       output : the height of the bounding box
*
*/
void boxConstruction_cpp(unsigned char* src, int width, int height, int *x_rect,int *y_rect, int *width_rect, int *height_rect);

/**
* Function used to convert the format of the image : RGB to WB format
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format) of the image to process
* @param dest
*       the array of size width*height with all the values of pixels(WB) of the image processed
*
*/
void saliencyFineGrained_cpp(unsigned char* src, unsigned char* dest, int width, int height);

void saliencySpectralRes_cpp(unsigned char* src, unsigned char* dest, int width, int height);

/**
* Function used to detect the water surface in a sequence of images of a swimmer.
 * In this function, we use a static array in which we've previously stored images without swimmer.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param nb_frames
*        number of frames of the init video stored in the static tab
* @param adjust_pt1
*        integer used to adjust the height of the left point of the line
* @param adjust_pt2
*        integer used to adjust the hieght of the right point of the line
*
*/
void surfaceDetection_cpp(unsigned char* src, unsigned char *image, unsigned char *dest, int width, int height);
void maskSurface_cpp(unsigned char* src, unsigned char * image, unsigned char *dest, int width, int height);

/**
* Function used to detect the swimmer in a frame using median blur filter, threshold and color filter on HSV components
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels of the image with the bounding box
*
*/
void swimmerAlgoDynamic_cpp(unsigned char *src, unsigned char *dest, int width, int height);

/**
* Function which returns the IOU : the area of intersection of 2 rect divided by hte area of the union of 2 rects
*
* @param rect1
 *      The real bounding box
* @param rect2
 *      The bounding box predicted by an algorithm
*
*/
double iouMetrics_cpp(Rect r1, Rect r2);

/**
* Function which returns an xarray of the norm Map computed between the 3 components RGB
*
* @param images
 *      xarray of unsigned char which contains the values of pixels of the image
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
*
*/
xt::xarray<unsigned char> normRGB(xt::xarray<unsigned char> images, int width, int height);
void cvtToWB_cpp(unsigned char* src, unsigned char* dest, int width, int height);
/**
* Function which returns a vector of Rect of bounding box
*
* @param path
 *      the path of the json file to read
*
*/
vector<Rect> readJson(string path);

#endif //SWIMMERDETECTION_C_IMAGE_H
