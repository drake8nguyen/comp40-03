/*
* run_transform.h
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "a2methods.h"
#include "pnm.h"


/* Functions that create the post-transformation images and call the apply 
functions that handle the transformations */

typedef struct closure *closure;

void new_image_wh(closure, Pnm_ppm*);
void new_image_hw(closure, Pnm_ppm*);
void swap_w_h(unsigned *, unsigned *);
void run_transform(int, A2Methods_mapfun*, closure, Pnm_ppm*);
