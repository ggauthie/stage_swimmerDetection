/*
============================================================================
Name        : splitMerge.c
Author      : kdesnos
Version     : 1.1
Copyright   : CECILL-C
Description :
============================================================================
*/

#include <string.h>
#include <stdlib.h>

#include "splitMerge.h"

void splitGray(int nbSlice, int width, int height, unsigned char *src, unsigned char *dest){
    int i;
    int sliceSize =  width*height/nbSlice;
    // Fill first and last line with 0
    memset(dest,0,width);
    // First Slice
    memcpy(dest+width, src, sliceSize);
    // Copy next line if several slice
    if (nbSlice > 1){
        memcpy(dest +  width + sliceSize , src + sliceSize, width);
    }
    // Slice other than first and last
    for(i=1; i<nbSlice-1; i++){
        int destIndex = i*(sliceSize+2*width);
        memcpy(dest + destIndex, src+i*sliceSize-width, sliceSize+2*width);
    }
    // Last Slice
    i = nbSlice-1;
    if(nbSlice > 1){
        // we have i = nbSlice -1;
        int destIndex = i*(sliceSize+2*width);
        memcpy(dest + destIndex, src+i*sliceSize-width, sliceSize+width);
    }
    // Last line
    memset(dest + (height+nbSlice*2-1)*width,0,width);
}

void split(int nbSlice, int width, int height, unsigned char *src, unsigned char *dest){
    int i;
    int sliceSize =  width*height*3/nbSlice;
    // Fill first and last line with 0
    memset(dest,0,width*3);
    // First Slice
    memcpy(dest+width*3, src, sliceSize);
    // Copy next line if several slice
    if (nbSlice > 1){
        memcpy(dest +  3*width + sliceSize , src + sliceSize, 3*width);
    }
    // Slice other than first and last
    for(i=1; i<nbSlice-1; i++){
        int destIndex = i*(sliceSize+2*(width*3));
        memcpy(dest + destIndex, src+i*sliceSize-(width*3), sliceSize+2*(width*3));
    }
    // Last Slice
    i = nbSlice-1;
    if(nbSlice > 1){
        // we have i = nbSlice -1;
        int destIndex = i*(sliceSize+2*(width*3));
        memcpy(dest + destIndex, src+i*sliceSize-(width*3), sliceSize+(width*3));
    }
    // Last line
    memset(dest + (height+nbSlice*2-1)*(width*3),0,width*3);
}


void mergeGray(int nbSlice, int width, int height, unsigned char *src, unsigned char *dest){
    int i;
    int sliceSize =  width*height/nbSlice;
    // Copy the slice content except the first and last lines
    for(i = 0; i< nbSlice; i++){
        int idx = i*(sliceSize +2*width);
        memcpy(dest+i*sliceSize, src+idx +width, sliceSize);
    }
}

void merge(int nbSlice, int width, int height, unsigned char *src, unsigned char *dest){
    int i;
    int sliceSize =  width*3*height/nbSlice;
    // Copy the slice content except the first and last lines
    for(i = 0; i< nbSlice; i++){
        int idx = i*(sliceSize+2*(width*3));
        memcpy(dest+i*sliceSize, src+idx+(width*3), sliceSize);
    }
}

