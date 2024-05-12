#pragma once

#ifndef _IMAGE
#define _IMAGE

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum allocation_type {
    NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED
};

typedef struct {
    int width;
    int height;
    int channels;
    size_t size;
    uint8_t *data;
    enum allocation_type allocation_;
} Image;

void Image_load(Image *img, const char *fname);
void Image_create(Image *img, int width, int height, int channels, bool zeroed);
void Image_save(const Image *img, const char *fname);
void Image_free(Image *img);
void Image_to_gray(const Image *orig, Image *gray);
void Image_to_sepia(const Image *orig, Image *sepia);
void Image_binary(const Image *orig, Image *binary);
void Image_negative(const Image *orig, Image *negative);
void Image_RGB2CMY(const Image *orig, Image *cmy);
void Image_get_pixel_values(const Image *img, uint8_t **pixel_values, size_t *size_vector);



#include  "Image.c"
#endif
