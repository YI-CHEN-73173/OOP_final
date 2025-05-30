#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <iostream>
#include <string>
#include "../Data-Loader/data_loader.h"
using namespace std;

class Image{
    protected:
        int w;//寬
        int h;//長
        static Data_Loader data_loader;

    public:
        Image();
        Image(int width, int height);
        virtual ~Image(){};
        int get_w();
        int get_h();

        virtual bool LoadImage(string filename)=0;
        virtual void DumpImage(string filename)=0;
        virtual void Display_X_Server()=0;
        virtual void Display_ASCII()=0;
        //virtual void Display_CMD()=0;
};

#endif