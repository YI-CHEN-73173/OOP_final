#ifndef _GRAY_IMAGE_H_
#define _GRAY_IMAGE_H_

#include <iostream>
#include <string>
#include "image.h"
using namespace std;

class GrayImage : public Image{
    private:
        int **pixels;
    public:
        GrayImage();
        GrayImage(int width, int height);
        GrayImage(int width,int height, int **p);
        GrayImage(const GrayImage &gi); // Copy constructor
        ~GrayImage();

        GrayImage& operator=(GrayImage &gi);   // Assignment operator
        // Override
        bool LoadImage(string filename); //加載圖片
        void DumpImage(string filename); //導出圖片(jpg/png)
        void Display_X_Server();//彈出視窗顯示圖片
        void Display_ASCII();//終端機顯示圖片
        //void Display_CMD();
        int **get_pixels(); //拿pixels
        void set_pixels(int w, int h, int p); //設定pixels
};

#endif