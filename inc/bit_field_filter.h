#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include<iostream>
#include "gray_image.h"
#include "rgb_image.h"

class BitFieldFilter {
    public :
        GrayImage* HorizontalFlip(GrayImage* img);
        GrayImage* MosaicFilter(GrayImage* img, int block_size);
        GrayImage* GaussianFilter(GrayImage* img, int kernel_size, double sigma);
        GrayImage* LaplacianFilter(GrayImage* img);
        // bonus
        GrayImage* FisheyeFilter(GrayImage* img);
        GrayImage* ImageRestoration(GrayImage* img);
        GrayImage* ImageRotate(GrayImage* img, float angle_degrees);
        GrayImage* HistogramSpecification(GrayImage* source, GrayImage* reference);

        RGBImage* HorizontalFlip(RGBImage* img);
        RGBImage* MosaicFilter(RGBImage* img, int block_size);    
        RGBImage* GaussianFilter(RGBImage* img, int kernel_size, double sigma);
        RGBImage* LaplacianFilter(RGBImage* img);
        // bonus
        RGBImage* FisheyeFilter(RGBImage* img);   
        RGBImage* ImageRestoration(RGBImage* img);
        RGBImage* ImageRotate(RGBImage* img, float angle_degrees);
        RGBImage* HistogramSpecification(RGBImage* source, RGBImage* reference);
     
};

#endif