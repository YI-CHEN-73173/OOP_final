#ifndef _RGB_IMAGE_H_
#define _RGB_IMAGE_H_

#include <iostream>
#include <string>
#include "image.h"
using namespace std;

class RGBImage : public Image{
    private:
        int ***pixels;
    public:
        RGBImage();
        RGBImage(int width, int height);
        RGBImage(int width, int height, int ***p);
        RGBImage(RGBImage &ri); // Copy constructor
        ~RGBImage();

        RGBImage& operator=(RGBImage &ri);   // Assignment operator
        // Override
        bool LoadImage(string filename);
        void DumpImage(string filename);
        void Display_X_Server();
        void Display_ASCII();
        //void Display_CMD();

        int ***get_pixels(); //拿pixels
        void set_pixels(int w, int h, int c, int p); //設定pixels
};

#endif