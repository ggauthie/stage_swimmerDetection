//
// Created by ggauthie on 31/05/2021.
//

#ifndef SWIMMERDETECTION_C_IMAGEWRAPPER_H
#define SWIMMERDETECTION_C_IMAGEWRAPPER_H

#pragma once

/*
 * use C name mangling if compiling as C++ code.
 * When compiling as C, this is ignored.
 */
#ifdef __cplusplus
extern "C" {
#endif
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
void cannyMethod(unsigned char *src, unsigned char* dest, int width, int height);

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
void otsuMethod(unsigned char *src, unsigned char* dest, int width, int height);
//void swimmerAlgo(unsigned char *src, unsigned char* dest, int width, int height, int *x_rect, int *y_rect, int *width_rect, int *height_rect);
void swimmerAlgo(unsigned char *src, unsigned char* dest, int width, int height);

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
void swimmerAlgoDynamic(unsigned char *src,unsigned char *dest, int width, int height);

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
void medianFilter(unsigned char *src, unsigned char *dest, int width, int height, int nb_channels);

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
void cvtColorHSV(unsigned char *src, unsigned char *dest, int width, int height);

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
void maskCreationSimple(unsigned char *src, unsigned char *dest, int width, int height);
void maskCreation(unsigned char *src, unsigned char* mask, unsigned char *dest, int width, int height);

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
void variance(unsigned char *src, unsigned char *dest, int width, int height, int nb_frames);

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
void variance_iter(unsigned char *src, unsigned char *dest, int width, int height);

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
void threshold(unsigned char *src, unsigned char *dest, int width, int height, int thresh);

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
void opening(unsigned char *src, unsigned char *dest, int width, int height, int k);

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
void closing(unsigned char *src, unsigned char *dest, int width, int height, int k);

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
void boxConstruction(unsigned char* src,int width, int height, int *x_rect,int *y_rect, int *width_rect, int *height_rect);

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
void cvtToWB(unsigned char* src, unsigned char* dest, int width, int height);

void saliencyFineGrained(unsigned char* src, unsigned char* dest, int width, int height);
void saliencySpectralRes(unsigned char* src, unsigned char* dest, int width, int height);
void surfaceDetection(unsigned char* src, unsigned char* dest, int width, int height);

/**
* Function which computes the norm between the 3 components RGB
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format) of the image to process
* @param dest
*       the array of size width*height with all the values of pixels(WB) of the norm
*
*/
void normRGB(unsigned char* src, unsigned char *dest, int width, int height);
#ifdef __cplusplus
}
#endif


#endif //SWIMMERDETECTION_C_IMAGEWRAPPER_H
