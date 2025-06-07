#ifndef _IMAGE_ENCRYPTION_H_
#define _IMAGE_ENCRYPTION_H_

#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <vector>
#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
using namespace std;

class ImageEncryption{
    public:
        
		//LSB
	    RGBImage* encode_LSB(string filename, string password);
        string decode_LSB(RGBImage* i);
  
		//XOR
		RGBImage* encode_XOR(string filename, string password); //預設key
	    RGBImage* encode_XOR(string filename, string password, string key); //user key
        string decode_XOR(RGBImage* image); //預設key
	    string decode_XOR(RGBImage* image, string key); //user key
		//Ceasar
		RGBImage* encode_Caesar(string filename, string password);
	    string decode_Caesar(RGBImage* image);
        //Substitution
        void generate_table();
        RGBImage* encode_Subs(string filename, string password);
        string decode_Subs(RGBImage* image);
        
	private:
		int C_shift = 3; // Caesar cipher shift
        map<int, int> subs; // Substitution cipher mapping
        map<int, int> reverse_subs;
};

#endif