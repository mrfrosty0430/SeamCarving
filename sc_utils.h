#ifndef SC_UTILS_H
#define SC_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

void write_image(const char* input, int width, int height,int comp,uint8_t* data, int stride);

uint8_t* read_image(const char* input, int *width_ptr, int *height_ptr,int* bpp_ptr);

void grey(const char* input, const char *output);

#endif