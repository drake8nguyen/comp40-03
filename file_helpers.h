/*
* file_helpers.h
* COMP 40 - Homework 3
* Andrew Gross & Minh Duc Nguyen
* October 9th, 2018
*/ 
#include <stdio.h>
#include <stdlib.h>

/* Functions to open and close a file */

FILE *open_file(const char *file_name);
void close_file(FILE *fp);