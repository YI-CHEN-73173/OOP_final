#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include<iostream>
#include "gray_image.h"
#include "rgb_image.h"

class BitFieldFilter {
    public :
        void HorizontalFlip(GrayImage* img);
        void MosaicFilter(GrayImage* img, int block_size);
        void GaussianFilter(GrayImage* img, int kernel_size, double sigma);
        void LaplacianFilter(GrayImage* img);
        // bonus
        void FisheyeFilter(GrayImage* img);
        void ImageRestoration(GrayImage* img);
        void ImageRotate(GrayImage* img, float angle_degrees);
        void HistogramSpecification(GrayImage* source, GrayImage* reference);

        void HorizontalFlip(RGBImage* img);
        void MosaicFilter(RGBImage* img, int block_size);    
        void GaussianFilter(RGBImage* img, int kernel_size, double sigma);
        void LaplacianFilter(RGBImage* img);
        // bonus
        void FisheyeFilter(RGBImage* img);   
        void ImageRestoration(RGBImage* img);
        void ImageRotate(RGBImage* img, float angle_degrees);
        void HistogramSpecification(RGBImage* source, RGBImage* reference);
     
};

#endif