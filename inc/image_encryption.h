#ifndef _IMAGE_ENCRYPTION_H_
#define _IMAGE_ENCRYPTION_H_

#include <iostream>
#include <string>
#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
using namespace std;

class ImageEncryption{
    public:
        RGBImage* encode_LSB(string filename, string password);
        string decode_LSB(RGBImage* i);
        

};

#endif