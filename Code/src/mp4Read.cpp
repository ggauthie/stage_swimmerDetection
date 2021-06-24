//
// Created by ggauthie on 28/05/2021.
//

#include "mp4Read.h"
#include "clock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FPS_MEAN 49
#define INBUF_SIZE 4096

static FILE *pipein;

static int id_frame = 0;
//static enum PixelFormat pix_fmt = RGB;
//static enum VideoFile video_file = SWIMMER;
static const char* url = "../data/Nage_Chrono_SV_1080_60fps_Cam1_cut_rescale.MP4";
static const char* urlLineDetection = "../data/lineDetect_rescale.mp4";
static const char* urlFullVideo = "../data/swimmer_rescale.mp4";


typedef struct RGBDisplay
{
    SDL_Texture* texture;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int stampId;
} RGBDisplay;

static RGBDisplay display;


void initMp4Read(enum VideoFile video_file, enum PixelFormat pix_fmt)
{
	char command[200] = "ffmpeg -i ";
    switch (video_file)
    {
        case INIT:
            strcat(command, urlLineDetection);
            break;
        case SWIMMER:
            strcat(command, url);
            break;
        case FULL_VIDEO:
        	strcat(command, urlFullVideo);
        	break;
        default :
            strcat(command, url);
            break;
    }
	strcat(command, " -f image2pipe -vcodec rawvideo -pix_fmt ");
	switch (pix_fmt)
	{
	    case RGB:
	        strcat(command, "rgb24 -");
	        break;
	    case YUV420:
	        strcat(command, "yuv420p -");
	        break;
	    default :
            strcat(command, "rgb24 -");
            break;
	 }
	    // Open an input pipe from ffmpeg
	pipein = popen(command, "r");
	printf("command : %s",command);
}

int mp4ReadRGB(int width, int height, unsigned char *r, unsigned char *g, unsigned char *b)
{
    int x, y, count;
    unsigned char frame[width*height*3];

    // Read a frame from the input pipe into the buffer
    count = fread(frame, 1, height*width*3, pipein);

// If we didn't get a frame of video, we're probably at the end
    if (count != height*width*3)
    {
            printf("End of the file");
            return 0;
    }
// Fill the 3 tabs r,g & b with the component of the frame (struc : RGB RGB RGB)
    else
    {
        for (y=0 ; y<height ; ++y) for (x=0 ; x<width ; ++x)
        {
                r[y*width+x] = frame[3*y*width+3*x];
                g[y*width+x] = frame[3*y*width+3*x + 1];
                b[y*width+x] = frame[3*y*width+3*x + 2];
        }
        //fwrite(frame, 1, height*width*3, pipeout);
        return count;
    }
}
int mp4ReadYUV(FILE* pipein, int width, int height, unsigned char *y,  unsigned char *u,  unsigned char *v)
{
    int i, j, res = 0;

// If we didn't get a frame of video, we're probably at the end
    if (res != height*width + height*width/2)
    {
        printf("End of the file");
        return 0;
    }
// Fill the 3 tabs r,g & b with the component of the frame (struc : RGB RGB RGB)
    else {
        res += fread(y, sizeof(char), width * height, pipein);
        res += fread(u, sizeof(char), width * height / 4, pipein);
        res += fread(v, sizeof(char), width * height / 4, pipein);
        return res;
    }
}

int mp4Read(int width, int height, unsigned char *pixels/*, unsigned char *output2*/)
{
    int count;

    count = fread(pixels, 1, height*width*3, pipein);
    //Double the output to resize the image at the end of the dataflow(not very clean)
    //memcpy(output2, pixels, height*width*3);
    if (count != height*width*3)
    {
        printf("End of the file");
        fflush(pipein);
        pclose(pipein);
        return count;
    }
    else
    {
        return count;
    }
}


void finalizeMp4Read()
{
    fflush(pipein);
    pclose(pipein);
}

void initMp4Display(int width, int height)
{
    //Step 1 : initialization
    if (SDL_Init(SDL_INIT_VIDEO) <0)
    {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    printf("SDL_Init_end\n");

    //Step 2 : Create the window

    display.window = SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    if(!display.window)
    {
        fprintf(stderr, "SDL: could not set video mode - exiting\n");
        exit(1);
    }

    //Step 3: Create the renderer

    display.renderer = SDL_CreateRenderer(display.window, -1, SDL_RENDERER_ACCELERATED);
    if (!display.renderer)
    {
        fprintf(stderr, "SDL: could not create renderer - exiting\n");
        exit(1);
    }

    /*display.stampId = 0;
    	for (int i = 0; i<FPS_MEAN; i++){
    		startTiming(i + 1);
    	}*/
}

void mp4DisplayRGB(int width, int height, unsigned char *r, unsigned char *g, unsigned char *b)
{
    SDL_Rect positionFond;

    display.texture= SDL_CreateTexture(display.renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING, width, height);
        if(!display.texture)
        {
            fprintf(stderr, "SDL: could not create texture - exiting\n");
            exit(1);
        }

    positionFond.w = width;
    positionFond.h = height;
    positionFond.x = width;
    positionFond.y = 0;

    SDL_QueryTexture(display.texture, NULL,NULL, &positionFond.x, &positionFond.y);

    //Fill an array of pixels 32bits
    Uint32 *img;
    SDL_PixelFormat *format;
    Uint32 *tmp;
    int pitch;

    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_LockTexture(display.texture, NULL, (void**)&tmp, &pitch);
    img=tmp;
    for(int y=0;y<height;y++)
    {
        for(int x=0; x< width; x++)
        {
            img[x+y*width] = SDL_MapRGB(format,r[y*width+x],g[y*width+x],b[y*width+x]);
        }
    }
    SDL_FreeFormat(format);
    SDL_UnlockTexture(display.texture);
    SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
    SDL_RenderPresent(display.renderer);
    SDL_Delay(100);
}

void mp4Display(int width, int height, unsigned char *pixels)
{
    SDL_Rect positionFond;
    //Step 4 : create a texture
    display.texture= SDL_CreateTexture(display.renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!display.texture)
    {
        fprintf(stderr, "SDL: could not create texture - exiting\n");
        exit(1);
    }

    positionFond.w = width;
    positionFond.h = height;
    positionFond.x = width;
    positionFond.y = 0;

    SDL_QueryTexture(display.texture, NULL,NULL, &positionFond.x, &positionFond.y);

    //Fill an array of pixels 32bits
    Uint32 *img;
    SDL_PixelFormat *format;
    Uint32 *tmp;
    int pitch;

    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_LockTexture(display.texture, NULL, (void**)&tmp, &pitch);
    img=tmp;
    for(int y=0;y<height;y++)
    {
        for(int x=0; x< width; x++)
        {
            img[x+y*width] = SDL_MapRGB(format,pixels[3*(y*width+x)],pixels[3*(y*width+x)+1],pixels[3*(y*width+x)+2]);
        }
    }
    SDL_FreeFormat(format);
    SDL_UnlockTexture(display.texture);
    SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
    SDL_RenderPresent(display.renderer);

    char fps_text[20];

    /*int time = stopTiming(display.stampId + 1);
    printf("FPS: %.2f", 1. / (time / 1000000. / FPS_MEAN));
    startTiming(display.stampId + 1);
    display.stampId = (display.stampId + 1) % FPS_MEAN;*/
}

void mp4DisplayWB(int width, int height, unsigned char* pixels)
{
    SDL_Rect positionFond;
    //Step 4 : create a texture
    display.texture= SDL_CreateTexture(display.renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!display.texture)
    {
        fprintf(stderr, "SDL: could not create texture - exiting\n");
        exit(1);
    }

    positionFond.w = width;
    positionFond.h = height;
    positionFond.x = width;
    positionFond.y = 0;

    SDL_QueryTexture(display.texture, NULL,NULL, &positionFond.x, &positionFond.y);

    //Fill an array of pixels 32bits
    Uint32 *img;
    SDL_PixelFormat *format;
    Uint32 *tmp;
    int pitch;

    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_LockTexture(display.texture, NULL, (void**)&tmp, &pitch);
    img=tmp;
    for(int y=0;y<height;y++)
    {
        for(int x=0; x< width; x++)
        {
            img[x+y*width] = SDL_MapRGB(format,pixels[(y*width+x)],pixels[(y*width+x)],pixels[(y*width+x)]);
        }
    }
    SDL_FreeFormat(format);
    SDL_UnlockTexture(display.texture);
    SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
    SDL_RenderPresent(display.renderer);
}

void finalizeMp4Display()
{
    //Destroy texture
    SDL_DestroyTexture(display.texture);
    //Destroy renderer
    SDL_DestroyRenderer(display.renderer);
    //Destroy window
    SDL_DestroyWindow(display.window);
    //Quit the SDL
    SDL_Quit();
}
