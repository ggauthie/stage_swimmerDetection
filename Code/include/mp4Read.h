//
// Created by ggauthie on 28/05/2021.
//

#ifndef SWIMMERDETECTION_C_MP4READ_H
#define SWIMMERDETECTION_C_MP4READ_H

#include <string.h>
#include "stdio.h"
#include "stdlib.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

/**
* Function used to read a MP4 file of size width*height*3 frame by frame.
* Unsigned char* r, g & b are filled with the values of each pixel component.
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param r
*        the red plane of the image of size width*height
* @param g
*        the green plane of the image of size width*height
* @param b
*        the blue plane of the image of size width*height
*/
int mp4ReadRGB(int width, int height, unsigned char *r, unsigned char *g, unsigned char *b);

/**
* Function used to read a MP4 file of size (width*height + width*height/2) frame by frame.
* Unsigned char* y, u & v are filled with the values of each pixel component.
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param y
*        the luma plane of the image of size width*height
* @param u
*        the chrominance(blue) plane of the image of size width*height/4
* @param v
*        the chrominance(red) plane of the image of size width*height/4
*/
int mp4ReadYUV(FILE* pipein, int width, int height, unsigned char *y,  unsigned char *u,  unsigned char *v);

/**
* Function used to read a MP4 file of size (3*width*height) frame by frame.
* Output with all the values of pixels.
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param pixels
*       the array of size 3*width*height with all the values of pixels of the image int this format (RGB RGB RGB...)
*
*/
int mp4Read(int width, int height, unsigned char *pixels);

/**
* Function used to init the pipe used to execute the command "ffmpeg" in the terminal..
*
*/
void initMp4Read();
/**
* Function used to close the pipe initialized.
*
*/
void finalizeMp4Read();

/**
* Function used to initialize the SDL objects useful to display images.
 *
* @param width
*        the width of the input image and SDLWindow
* @param height
*        the height of the input image and SDLWindow
*/
void initMp4Display(int width, int height);

/**
* Function used to close the SDL and delete its associated objects(Window, Renderer etc..).
*
*/
void finalizeMp4Display();

/**
* Function used to display an image of size (3*width*height).
* The input is 3 arrays of size width*height of 3 different components R,G and B
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param r
*       the array of size width*height with all the values of component Red of pixels of the image
* @param g
*       the array of size width*height with all the values of component green of pixels of the image
* @param b
*       the array of size width*height with all the values of component blue of pixels of the image
*
*/

void mp4DisplayRGB( int width, int height, unsigned char *r, unsigned char *g, unsigned char *b);

/**
* Function used to display an image of size (3*width*height).
* The input is an array of size 3*width*height int this pixel format : (RGB RGB ...)
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param pixels
*       the array of size 3*width*height with all the values of pixels of the image int this format (RGB RGB RGB...)
*
*/
void mp4Display(int width, int height, unsigned char* pixels);

/**
* Function used to display an image of size (width*height).
* The input is an array of size width*height of the gray value of the pixel
*
* @param width
*        the width of the input image
* @param height
*        the height of the input image
* @param pixels
*       the array of size width*height with all the values of pixels of the image
*
*/
void mp4DisplayWB(int width, int height, unsigned char* pixels);

#endif //SWIMMERDETECTION_C_MP4READ_H
