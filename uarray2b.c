/*
* urray2b.c
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/

#include <stdlib.h>
#include <stdio.h>
#include "uarray2.h"
#include "uarray2b.h"
#include <assert.h>
#include <math.h>
#include "uarray.h"

struct UArray2b_T {
        int WIDTH;
        int HEIGHT;
        int c_size;
        int blk_size;
        UArray2_T ray;
        int c_width;
        int c_height;
};

const int MAX_SIZE = 65536;

UArray2b_T UArray2b_new (int width, int height, int size, int blocksize)
{
        assert(blocksize > 0);
        assert(width > 0 && height > 0);
        assert(size > 0);

        UArray2b_T block_array = malloc(sizeof(struct UArray2b_T));
        assert(block_array);

        int cells_length = blocksize*blocksize;
        block_array->c_size = size;
        block_array->blk_size = blocksize;
        block_array->WIDTH = (int)ceil((double)width/(double)blocksize);
        block_array->HEIGHT = (int)ceil((double)height/(double)blocksize);
        block_array->c_height = height;
        block_array->c_width = width;
        block_array->ray = UArray2_new(block_array->WIDTH, block_array->HEIGHT,
                                        sizeof(UArray_T));
        for (int y = 0; y < block_array->HEIGHT; y++) {
                for (int x = 0; x < block_array->WIDTH; x++) {
                        UArray_T* a_block = (UArray_T*) UArray2_at(
                                                        block_array->ray, x, y);
                        *a_block = UArray_new(cells_length, size);
                }       
        }
        return block_array;
}

UArray2b_T UArray2b_new_64K_block(int width, int height, int size)
{
        assert(width > 0 && height > 0);
        assert(size > 0);
        int blocksize;

        if (size > MAX_SIZE) {
                blocksize = 1;
        } else if (size < MAX_SIZE) {
                blocksize = (int)sqrt(MAX_SIZE/size);
        }
        return UArray2b_new(width, height, size, blocksize);
}

int UArray2b_width (UArray2b_T array2b)
{
        assert(array2b);
        return array2b->c_width;
}

int UArray2b_height (UArray2b_T array2b)
{
        assert(array2b);
        return array2b->c_height;
}

int UArray2b_size (UArray2b_T array2b)
{
        assert(array2b);
        return array2b->c_size;
}

int UArray2b_blocksize(UArray2b_T array2b)
{
        assert(array2b);
        return array2b->blk_size;
}

void *UArray2b_at(UArray2b_T array2b, int column, int row)
{
        assert(array2b);
        assert(column >= 0);
        assert(row >= 0);

        // assert(column < array2b->c_width);
        // assert(row < array2b->c_height);

        int bs = array2b->blk_size;
        int blk_col = column/bs; 
        int blk_row = row/bs;

        void *blk_ray = UArray2_at(array2b->ray, blk_col, blk_row);
        int i = bs*(row%bs) + column%bs;
        void *cell = UArray_at(*(UArray_T*)blk_ray, i);

        /*edge case (outermost column)*/
        if ((blk_col == array2b->WIDTH-1) && (blk_row < array2b->HEIGHT-1)) {
                if (array2b->c_width % bs == 0) {
                        return cell;
                } else if (i % bs > (array2b->c_width % bs - 1)) {
                        return NULL;
                } 
        /*edge case (lowermost row)*/
        } else if ((blk_col < array2b->WIDTH-1) && 
                   (blk_row == array2b->HEIGHT-1)) {
            
                if (array2b->c_height % bs == 0) {
                        return cell;
                } else if (i > (array2b->c_height % bs) * bs - 1) {
                        return NULL;
                }
        /*edge case (bottom right corner)*/
        } else if ((blk_row == array2b->HEIGHT-1) && 
                   (blk_col == array2b->WIDTH-1)) {
                
                if (array2b->c_width % bs == 0 && array2b->c_height % bs == 0) {
                        return cell;
                }
                else if (array2b->c_width % bs == 0) {
                        if (i > (array2b->c_height % bs) * bs - 1) {
                                return NULL;   
                        }
                }
                else if (array2b->c_height % bs == 0) {
                        if (i % bs > (array2b->c_width % bs - 1)) {
                                return NULL;
                        }
                }
                else {
                        if (i > (array2b->c_height % bs) * bs - 1) {
                                return NULL;   
                        }
                        
                        if (i % bs > ((array2b->c_width % bs) - 1)) {
                                return NULL;
                        }
                }
        }
        return cell;
}


void apply_block(UArray2b_T array2b, void* this_block, int b_col, int b_row,
                void apply(int col, int row, UArray2b_T array2b, void *elem, 
                           void *cl), void *cl)
{
        assert(array2b);
        assert(this_block);
        int array_size = array2b->blk_size * array2b->blk_size;

        for (int i = 0; i < array_size; i++) {
                int c_col = array2b->blk_size * b_col + i % array2b->blk_size;
                int c_row = array2b->blk_size * b_row + i / array2b->blk_size;
                void* elem = UArray2b_at(array2b, c_col, c_row);

                if (elem == NULL) {
                        continue;
                }
                apply(c_col, c_row, array2b, elem, cl);
        }
}

void UArray2b_map(UArray2b_T array2b, 
                void apply(int col, int row, UArray2b_T array2b, void *elem, 
                           void *cl), void *cl)
{
        assert(array2b);
        for (int b_row = 0; b_row < array2b->HEIGHT; b_row++) {
                for (int b_col = 0; b_col < array2b->WIDTH; b_col++) { 
                        void *this_block = UArray2_at(array2b->ray, b_col,
                                                      b_row);
                        apply_block(array2b, this_block, b_col, b_row, apply,
                                    cl);
                }
        }
}

void UArray2b_free (UArray2b_T *array2b)
{
    assert(*array2b);    
    for (int i = 0; i < (*array2b)->WIDTH; i++) {
        for (int j = 0; j < (*array2b)->HEIGHT; j++) {
            UArray_T *the_block = UArray2_at((*array2b)->ray, i, j);
            UArray_free(the_block);
        }
    }
    UArray2_free(&((*array2b)->ray));
    free(*array2b);
}