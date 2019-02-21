/*
* apply_functs.h
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "a2methods.h"

/* Different apply functions for different transformations */

void apply_0(int, int, A2Methods_UArray2, void*, void*);
void apply_90(int, int, A2Methods_UArray2, void*, void*);
void apply_180(int, int, A2Methods_UArray2, void*, void*);
void apply_270(int, int, A2Methods_UArray2, void*, void*);
void mirror_lr(int, int, A2Methods_UArray2, void*, void*);
void mirror_ud(int, int, A2Methods_UArray2, void*, void*);