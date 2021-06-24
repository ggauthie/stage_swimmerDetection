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

void splitGray(int nbSlice, int width, int height, unsigned char *input, unsigned char *output){
    int i;
    int sliceSize =  width*height/nbSlice;
    // Fill first and last line with 0
    memset(output,0,width);
    // First Slice
    memcpy(output+width, input, sliceSize);
    // Copy next line if several slice
    if (nbSlice > 1){
        memcpy(output +  width + sliceSize , input + sliceSize, width);
    }
    // Slice other than first and last
    for(i=1; i<nbSlice-1; i++){
        int destIndex = i*(sliceSize+2*width);
        memcpy(output + destIndex, input+i*sliceSize-width, sliceSize+2*width);
    }
    // Last Slice
    i = nbSlice-1;
    if(nbSlice > 1){
        // we have i = nbSlice -1;
        int destIndex = i*(sliceSize+2*width);
        memcpy(output + destIndex, input+i*sliceSize-width, sliceSize+width);
    }
    // Last line
    memset(output + (height+nbSlice*2-1)*width,0,width);
}

void split(int nbSlice, int width, int height, unsigned char *input, unsigned char *output){
    int i;
    int sliceSize =  width*height*3/nbSlice;
    // Fill first and last line with 0
    memset(output,0,width*3);
    // First Slice
    memcpy(output+width*3, input, sliceSize);
    // Copy next line if several slice
    if (nbSlice > 1){
        memcpy(output +  3*width + sliceSize , input + sliceSize, 3*width);
    }
    // Slice other than first and last
    for(i=1; i<nbSlice-1; i++){
        int destIndex = i*(sliceSize+2*(width*3));
        memcpy(output + destIndex, input+i*sliceSize-(width*3), sliceSize+2*(width*3));
    }
    // Last Slice
    i = nbSlice-1;
    if(nbSlice > 1){
        // we have i = nbSlice -1;
        int destIndex = i*(sliceSize+2*(width*3));
        memcpy(output + destIndex, input+i*sliceSize-(width*3), sliceSize+(width*3));
    }
    // Last line
    memset(output + (height+nbSlice*2-1)*(width*3),0,width*3);
}
void mergeGray(int nbSlice, int width, int height, unsigned char *input, unsigned char *output){
    int i;
    int sliceSize =  width*height/nbSlice;
    // Copy the slice content except the first and last lines
    for(i = 0; i< nbSlice; i++){
        int idx = i*(sliceSize+2*width);
        memcpy(output+i*sliceSize, input+idx+width, sliceSize);
    }
}

void merge(int nbSlice, int width, int height, unsigned char *input, unsigned char *output){
    int i;
    int sliceSize =  width*3*height/nbSlice;
    // Copy the slice content except the first and last lines
    for(i = 0; i< nbSlice; i++){
        int idx = i*(sliceSize+2*(width*3));
        memcpy(output+i*sliceSize, input+idx+(width*3), sliceSize);
    }
}