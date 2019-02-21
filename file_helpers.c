/*
* file_helpers.c
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/
#include "file_helpers.h"
#include <stdio.h>
#include <stdlib.h>

/*open file*/
FILE *open_file(const char *file_name)
{
        FILE *fp;
        if (file_name != NULL) {
                fp = fopen(file_name, "rb");
                if (fp == NULL) {
                        perror(file_name);
                        exit(EXIT_FAILURE);
                }
        } else {
                fp = stdin;
        }
        return fp;
}

/*close file*/
void close_file(FILE *fp)
{
        if (fp != stdin) {
                if (fclose(fp) != 0) {
                        exit(1);
                }
        }
}