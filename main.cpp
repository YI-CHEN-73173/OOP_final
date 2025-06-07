#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"

#include<iostream>
using namespace std;
# define CASE_FLIP         0b00000001
# define CASE_MOSAIC       0b00000010
# define CASE_GAUSSIAN     0b00000100
# define CASE_LAPLACIAN    0b00001000
# define CASE_FISHEYE      0b00010000
# define CASE_RESTORATION  0b00100000
# define CASE_ROTATION     0b01000000
# define CASE_SPECIFICATION 0b10000000

void loadCase(int8_t option, GrayImage* img, BitFieldFilter* filter)
{
    if (option & CASE_FLIP) {
        GrayImage* flipped = filter->HorizontalFlip(img);
        flipped->Display_X_Server();
        delete flipped;
    }
    if (option & CASE_MOSAIC) {
        GrayImage* mosaic = filter->MosaicFilter(img, 10);
        mosaic->Display_X_Server();
        delete mosaic;
    }
    if (option & CASE_GAUSSIAN) {
        GrayImage* gaussian = filter->GaussianFilter(img, 5, 1.0);
        gaussian->Display_X_Server();
        delete gaussian;
    }
    if (option & CASE_LAPLACIAN) {
        GrayImage* laplacian = filter->LaplacianFilter(img);
        laplacian->Display_X_Server();
        delete laplacian;
    }
    if (option & CASE_FISHEYE) {
        GrayImage* fisheye = filter->FisheyeFilter(img);
        fisheye->Display_X_Server();
        delete fisheye;
    }
    if (option & CASE_RESTORATION) {
        GrayImage* restoration = filter->ImageRestoration(img);
        restoration->Display_X_Server();
        delete restoration;
    }
    if (option & CASE_ROTATION) {
        GrayImage* rotated = filter->ImageRotate(img, 30.0f);
        rotated->Display_X_Server();
        delete rotated;
    }
}
void loadCase(int8_t option, GrayImage* img1, GrayImage* img2, BitFieldFilter* filter)
{
    if (option & CASE_SPECIFICATION) {
        GrayImage* specification = filter->HistogramSpecification(img1, img2);
        //specification->Display_X_Server();
        delete specification;
    }
}

void loadCase(int8_t option, RGBImage* img, BitFieldFilter* filter)
{
    if (option & CASE_FLIP) {
        RGBImage* flipped = filter->HorizontalFlip(img);
        flipped->Display_X_Server();
        delete flipped;
    }
    if (option & CASE_MOSAIC) {
        RGBImage* mosaic = filter->MosaicFilter(img, 10);
        mosaic->Display_X_Server();
        delete mosaic;
    }
    if (option & CASE_GAUSSIAN) {
        RGBImage* gaussian = filter->GaussianFilter(img, 5, 1.0);
        gaussian->Display_X_Server();
        delete gaussian;
    }
    if (option & CASE_LAPLACIAN) {
        RGBImage* laplacian = filter->LaplacianFilter(img);
        laplacian->Display_X_Server();
        delete laplacian;
    }
    if (option & CASE_FISHEYE) {
        RGBImage* fisheye = filter->FisheyeFilter(img);
        fisheye->Display_X_Server();
        delete fisheye;
    }
    if (option & CASE_RESTORATION) {
        RGBImage* restoration = filter->ImageRestoration(img);
        restoration->Display_X_Server();
        delete restoration;
    }
    if (option & CASE_ROTATION) {
        RGBImage* rotated = filter->ImageRotate(img, 30.0f);
        rotated->Display_X_Server();
        delete rotated;
    }
}
void loadCase(int8_t option, RGBImage* img1, RGBImage* img2, BitFieldFilter* filter)
{
    if (option & CASE_SPECIFICATION) {
        RGBImage* specification = filter->HistogramSpecification(img1, img2);
        //specification->Display_X_Server();
        delete specification;
    }
}

int main(int argc, char *argv[]){
/*
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    img1->Display_ASCII();
    

    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    img2->Display_ASCII();
*/
        
    //RGBImage* imgtest = new RGBImage();
    RGBImage* img1 = new RGBImage();
    RGBImage* img2 = new RGBImage();
    img1->LoadImage("Image-Folder/2-2.jpg");
    img2->LoadImage("Image-Folder/lena.jpg");
    //img1->Display_X_Server();
    //img2->Display_X_Server();
    //imgtest->LoadImage("Image-Folder/lena.jpg");
    //imgtest->LoadImage("Image-Folder/truck.png");
    //imgtest->Display_X_Server();
    
    BitFieldFilter* filter = new BitFieldFilter();
    
    
    
    int8_t option = CASE_SPECIFICATION; 
    //loadCase(option, imgtest, filter);
    loadCase(option, img1, img2, filter);
    // some photo mosaic driven code here

    // more ...
   
    //warping
    
    
    delete img1;
    delete img2;
    delete filter;
    //delete imgtest;
    return 0;
}
