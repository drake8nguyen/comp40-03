/*
* ppmtrans.c
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
#include "cputiming.h"
#include "apply_functs.h"
#include "file_helpers.h"
#include "run_transform.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

struct closure {
        A2Methods_T the_methods;
        A2Methods_UArray2 new_image;

};

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        int   i;
        double time_used;
        double time_per_pix;
        char *major;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        char *file_name;
        FILE *timings_file = NULL;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                            "row-major");
                        major = argv[i];
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                            "column-major");
                        major = argv[i];                
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                        major = argv[i];
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                "Rotation must be 0, 90, 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if (strcmp(argv[++i], "horizontal") == 0) {
                                rotation = 1;
                        } else if (strcmp(argv[i], "vertical") == 0) {
                                rotation = 2;
                        }
                } else if (strcmp(argv[i], "-transpose") == 0) {
                                usage(argv[0]);
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                        timings_file = fopen(time_file_name, "a");
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else if (argc - i == 1) {
                        file_name = argv[i]; 
                }
                else {
                        break;
                }
        }

        FILE *fp = open_file(file_name);
        Pnm_ppm orig_image = Pnm_ppmread(fp, methods);
        struct closure cl;
        cl.the_methods = methods;

        CPUTime_T timer;
        timer = CPUTime_New();

        CPUTime_Start(timer);
        run_transform(rotation, map, &cl, &orig_image);
        time_used = CPUTime_Stop(timer);
        time_per_pix = time_used/(orig_image->width * orig_image->height);

        /*outputting timer result*/
        if (timings_file){
            fprintf(timings_file, "%d x %d ppm %s rotated %s %d degrees "
                    "in %.0f ns.\n Time/pixel was %.0f ns\n\n", 
                    orig_image->width, orig_image->height, file_name, 
                    major, rotation, time_used, time_per_pix);
        }
        CPUTime_Free(&timer);

        methods->free(&orig_image->pixels);
        orig_image->pixels = cl.new_image;
        Pnm_ppmwrite(stdout, orig_image);
        close_file(fp);

        if(timings_file != NULL) {
                close_file(timings_file);
        }
        Pnm_ppmfree(&orig_image);
        exit(0);
}




