/*
* apply_functs.c
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"


struct closure {
        A2Methods_T the_methods;
        A2Methods_UArray2 new_image;

};

/*rotate 90 degree*/
void apply_90(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;
        int h = methods->height(pixels);

        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = (h-j-1);
        int new_j = i;
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}

/*rotate 180 degree*/
void apply_180(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;
        int w = methods->width(pixels);
        int h = methods->height(pixels);
        
        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = (w-i-1);
        int new_j = (h-j-1);
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}

/*rotate 270 degree*/
void apply_270(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;
        int w = methods->width(pixels);

        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = j;
        int new_j = (w-i-1);
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}

/*rotate 0 degree*/
void apply_0(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;        
        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = i;
        int new_j = j;
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}

/*flip horizontally*/
void mirror_lr(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;
        int w = methods->width(pixels);
        
        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = (w-i-1);
        int new_j = j;
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}

/*flip vertically*/
void mirror_ud(int i, int j, A2Methods_UArray2 pixels, void* elem,
                            void* cl)
{
        A2Methods_T methods = ((struct closure*)cl)->the_methods;
        int h = methods->height(pixels);
        
        elem = methods->at(pixels, i, j);
        A2Methods_UArray2* new_image = ((struct closure*)cl)->new_image;
        int new_i = i;
        int new_j = (h-j-1);
        *(Pnm_rgb) methods->at(new_image, new_i, new_j) = *(Pnm_rgb)elem;
}