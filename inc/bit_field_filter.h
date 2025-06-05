#ifndef BIT_FIELD_FILTER_H
#define BIT_FIELD_FILTER_H

#include<iostream>
#include "gray_image.h"
#include "rgb_image.h"

class BitFieldFilter {
    public :
        void GrayImage HorizontalFlip(GrayImage &img);
        void GrayImage MosaicFlip(GrayImage &img, int block_size);
        void GrayImage GaussianFilter(GrayImage &img, int kernel_size, double sigma);
        void GrayImage LaplacianFilter(GrayImage &img);

        void RGBImage HorizontalFlip(RGBImage &img);
        void RGBImage MosaicFlip(RGBImage &img, int block_size);    
        void RGBImage GaussianFilter(RGBImage &img, int kernel_size, double sigma);
        void RGBImage LaplacianFilter(RGBImage &img);        
}