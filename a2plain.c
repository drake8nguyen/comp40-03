/*
* a2plain.c
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/
#include <stdlib.h>
#include <a2plain.h>
#include "uarray2.h"
#include <a2methods.h>

/* Define a private version of each function in A2Methods_T that we implement 
by calling functions from UArray2_T */

typedef A2Methods_UArray2 A2;

static A2Methods_UArray2 new(int width, int height, int size)
{
    return UArray2_new(width, height, size);
}
static A2Methods_UArray2 new_with_blocksize(int width, int height,
int size, int blocksize)
{
    (void) blocksize;
    return UArray2_new(width, height, size);
}

static void a2free(A2 *array2p)
{
    UArray2_free((UArray2_T *) array2p); 
}

static int width(A2 array2)
{
    return UArray2_width(array2);
}

static int height(A2 array2)
{
    return UArray2_height(array2);
}

static int blocksize(A2 array2)
{
    (void) array2;
    return 1;
}

static int size(A2 array2)
{
    return UArray2_size(array2);
}

static A2Methods_Object *at(A2 array2, int i, int j)
{
    return UArray2_at(array2, i, j);
}

typedef void UArray2_apply(int col, int row, UArray2_T array2, 
               void *elem, void *cl);

extern void UArray2_map_row_major(UArray2_T a2, UArray2_apply apply, void *cl);
extern void UArray2_map_col_major(UArray2_T a2, UArray2_apply apply, void *cl);

struct a2fun_closure {
    A2Methods_smallapplyfun *apply;
    void *cl;
    A2Methods_UArray2 array2;
};


static void apply_a2methods_using_array2_prototype(int col, int row, 
                UArray2_T array2, void *elem, void *cl)
{
    struct a2fun_closure *f = cl;
    (void) row;
    (void) col;
    (void) array2;
    f->apply(elem, f->cl);
}

static void map_row_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
    UArray2_map_row_major(array2, (UArray2_apply*)apply, cl);
}
static void map_col_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
    UArray2_map_col_major(array2, (UArray2_apply*) apply, cl);  
}


static void small_map_row_major(A2 a2, A2Methods_smallapplyfun apply, void *cl)
{
    struct a2fun_closure mycl = { apply, cl, a2 };
    UArray2_map_row_major(a2, apply_a2methods_using_array2_prototype, 
                  &mycl);
}

static void small_map_col_major(A2 a2, A2Methods_smallapplyfun apply, void *cl)
{
    struct a2fun_closure mycl = { apply, cl, a2 };
    UArray2_map_col_major(a2, apply_a2methods_using_array2_prototype, 
                  &mycl);
}

static struct A2Methods_T uarray2_methods_plain_struct = {
    new,
    new_with_blocksize,
    a2free,
    width,
    height,
    size,
    blocksize,
    at,
    map_row_major,
    map_col_major,
    NULL,           
    map_row_major,  //map_default
    small_map_row_major,
    small_map_col_major,
    NULL,       
    small_map_row_major, //small_map_default

};

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;