//
// Created by ggauthie on 31/05/2021.
//

#ifndef SWIMMERDETECTION_C_IMAGEWRAPPER_H
#define SWIMMERDETECTION_C_IMAGEWRAPPER_H

#include "rectangle.h"

/*
 * use C name mangling if compiling as C++ code.
 * When compiling as C, this is ignored.
 */
#ifdef __cplusplus
extern "C" {
#endif

void swimmerAlgo(int width, int height, unsigned char *src, unsigned char *dest);

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
void swimmerAlgoDynamic(int width, int height, unsigned char *src, unsigned char *dest, Rectangle *box);

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
void medianFilter(int width, int height, int nb_channels, unsigned char *src, unsigned char *dest);

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
void cvtColorHSV(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to apply the mask which delete the part on top of the water surface:
* The mask of the surface is stored in static
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size width*height with all the values of pixels of the mask to process
* @param dest
*       output : the array of size width*height with all the values of pixels of the final mask created
*
*/
void applyMaskSurface(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to detect the very lightly zone in the image, and generate a mask without these zone.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*        Image input(HSV) of size 3*height*width
* @param dest
*        Mask generated(WB) of size height*width
*
*/
void deleteLight(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to apply a mask on an Image.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*        Input Image of size nb_channels*height*width
* @param mask
*        Input mask of size height*width applied on the input image
* @param dest
*        Image Generated of size nb_channels*height*width
* @param nb_channels
*        int, Number of the channels 3 for YUV,RGB and HSV 1 for WandB image
*
*/
void bitwise_and(unsigned char *src, unsigned char * mask, unsigned char *dest, int width, int height, int nb_channels);

/**
* Function used to combine two rectangles. Intersection of the two rectangles.
*
* @param box1
*        First Rectangle to combine
* @param box2
*        Second Rectangle to combine
* @param boxMerged
*        Rectangle resulted
*/
void mergeBox(Rectangle *box1, Rectangle *box2, Rectangle *boxMerged);

/**
* Function used to draw a rectangle one source Image. The image resulted is the dest Image.
*
* @param src
*        Input Image
* @param dest
*        Output Image
* @param bb
*        Rectangle to draw
* @param width
*        width of the image
* @param height
*        height of the image
*/
void drawRectangle(int width, int height, unsigned char *src, Rectangle* bb, unsigned char *dest);

/**
* Function which returns the IOU : the area of intersection of 2 Rect divided by hte area of the union of 2 rects
*
* @param r1
 *      First Rectangle
* @param r2
 *      2nd Rectangle
*
*/
void iou(Rectangle *r1, Rectangle* r2, float *iou);

/**
* Function which returns the IOU : the area of intersection of 2 Rect divided by hte area of the union of 2 rects
*
* @param r1
 *      First Rectangle
* @param r2
 *      2nd Rectangle
*
*/
float iou_1(Rectangle *r1, Rectangle* r2);

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
void variance(int width, int height, int nb_frames, unsigned char *src, unsigned char *dest);

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
void variance_iter(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function which applies a threshold operation on an image. 
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param thresh
*        a value between 0 and 255
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format)
* @param dest
*       output : the array of size 3*width*height with all the values of pixels of the image
*
*/
void threshold(int width, int height, int thresh, unsigned char *src, unsigned char *dest);

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
void opening(int width, int height, int k, unsigned char *src, unsigned char *dest);

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
void closing(int width, int height, int k, unsigned char *src, unsigned char *dest);

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
void boxConstruction(int width, int height, unsigned char *src, Rectangle* bb);

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
void cvtToWB(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to detect the water surface in a sequence of images of a swimmer.
 * In this function, we use a static array in which we've previously stored images without swimmer.
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*        norm of the variance previously calculated
* @param adjust_pt1
*        integer used to adjust the height of the left point of the line
* @param adjust_pt2
*        integer used to adjust the hieght of the right point of the line
*
*/
void generateMaskSurface(int width, int height, unsigned char *src);

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
void normRGB(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to create a mask highlighting the swimmer in the image :
* Process :
* - Filter the 3 components Y,U,V to create a mask
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(YUV format) of the image to process
* @param dest
*       output : the array of size width*height with all the values of pixels of the final mask created
*
*/
void segmentationYUV(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to convert the format of the image : RGB to YUV format
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(RGB format) of the image to process
* @param dest
*       the array of size 3*width*height with all the values of pixels(YUV) of the image processed
*
*/
void cvtColorRGBtoYUV(int width, int height, unsigned char *src, unsigned char *dest);

/**
* Function used to create a mask highlighting the swimmer in the image :
* Process :
* - Filter the 3 components H,S,V to create a mask
*
* @param width
*        the width of the input/output image
* @param height
*        the height of the input/output image
* @param src
*       the array of size 3*width*height with all the values of pixels(YUV format) of the image to process
* @param dest
*       output : the array of size width*height with all the values of pixels of the final mask created
*
*/
void segmentationHSV(int width, int height, unsigned char *src, unsigned char *dest);

#ifdef __cplusplus
}
#endif

#endif //SWIMMERDETECTION_C_IMAGEWRAPPER_H
