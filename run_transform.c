/*
* run_transform.c
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
#include "run_transform.h"
#include "pnm.h"
#include "apply_functs.h"
#include "file_helpers.h"

struct closure {
        A2Methods_T the_methods;
        A2Methods_UArray2 new_image;

};

/*create new image with same dimensions for 180 rotation and flipping*/
void new_image_wh(struct closure *cl, Pnm_ppm *orig_image)
{
        
        A2Methods_T methods = cl->the_methods;
        cl->new_image = methods->new(
                methods->width((*orig_image)->pixels), 
                methods->height((*orig_image)->pixels),
                methods->size((*orig_image)->pixels));
}

/*create new image with reverse dimensions for 90 and 270 degree rotation*/
void new_image_hw(struct closure *cl, Pnm_ppm *orig_image)
{
        
        A2Methods_T methods = cl->the_methods;
        cl->new_image = methods->new(
                        methods->height((*orig_image)->pixels), 
                        methods->width((*orig_image)->pixels),
                        methods->size((*orig_image)->pixels));
}

/*helper function to switch width and height of the Pnm_ppm*/
void swap_w_h(unsigned* w, unsigned* h)
{
        unsigned temp;
        temp = *w;
        *w = *h;
        *h = temp;
}

/*where the magic happens*/
void run_transform(int rotation, A2Methods_mapfun* map, struct closure *cl, 
                                        Pnm_ppm* orig_image)
{
        if (rotation == 90){
                new_image_hw(cl, orig_image);
                map((*orig_image)->pixels, apply_90, cl); 
                swap_w_h(&(*orig_image)->width, &(*orig_image)->height);
        } else if (rotation == 180) {
                new_image_wh(cl, orig_image);
                map((*orig_image)->pixels, apply_180, cl); 
        } else if (rotation == 0) {
                new_image_wh(cl, orig_image);
                map((*orig_image)->pixels, apply_0, cl); 
        } else if (rotation == 270){
                new_image_hw(cl, orig_image);
                map((*orig_image)->pixels, apply_270, cl); 
                swap_w_h(&(*orig_image)->width, &(*orig_image)->height);
        } else if (rotation == 1) {
                new_image_wh(cl, orig_image);
                map((*orig_image)->pixels, mirror_lr, cl); 
        } else if (rotation == 2) {
                new_image_wh(cl, orig_image);
                map((*orig_image)->pixels, mirror_ud, cl); 
        }
}