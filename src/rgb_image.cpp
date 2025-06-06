#include<iostream>
#include<string>
#include "rgb_image.h"
using namespace std;

RGBImage::RGBImage() : Image() {
    pixels = nullptr;
}
RGBImage::RGBImage(int width, int height) : Image(width, height) {
    pixels = new int**[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new int*[width];
        for (int j = 0; j < width; ++j) {
            pixels[i][j] = new int[3]; // 3 channels for RGB
        }
    }
}
RGBImage::RGBImage(int width, int height, int ***p) : Image(width, height) {
    pixels = new int**[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = new int[3]; // 3 channels for RGB
            for (int c = 0; c < 3; ++c) {
                pixels[i][j][c] = p[i][j][c];
            }
        }
    }
}
RGBImage::RGBImage(RGBImage &ri) : Image(ri.get_w(), ri.get_h()) {
    pixels = new int**[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = new int[3]; // 3 channels for RGB
            for (int c = 0; c < 3; ++c) {
                pixels[i][j][c] = ri.pixels[i][j][c];
            }
        }
    }
}
RGBImage::~RGBImage() {
    if (pixels != nullptr) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                delete[] pixels[i][j];
            }
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    pixels = nullptr;
}
RGBImage& RGBImage::operator=(RGBImage &ri) {
    if (this == &ri) return *this;
    if (pixels != nullptr) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                delete[] pixels[i][j];
            }
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    
    h = ri.get_h();
    w = ri.get_w();
    // Allocate new memory
    pixels = new int**[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = new int[3]; // 3 channels for RGB
            for (int c = 0; c < 3; ++c) {
                pixels[i][j][c] = ri.pixels[i][j][c];
            }
        }
    }
    return *this;
}
bool RGBImage::LoadImage(string filename) {
    if (pixels != nullptr) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                delete[] pixels[i][j];
            }
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    pixels = data_loader.Load_RGB(filename, &w, &h);
    return pixels != nullptr; //若成功加載圖片則回傳1
}
void RGBImage::DumpImage(string filename) {
    data_loader.Dump_RGB(w, h, pixels, filename);
}
void RGBImage::Display_X_Server() {
    data_loader.Display_RGB_X_Server(w, h, pixels);
}
void RGBImage::Display_ASCII() {
    data_loader.Display_RGB_ASCII(w, h, pixels);
}
int ***RGBImage::get_pixels() {
    return pixels;
}
void RGBImage::set_pixels(int h, int w, int c, int p) {
    if (pixels == nullptr) {
        cout << "Error: no image." << endl;
    }
    else{
        pixels[h][w][c] = p;
    }
    return;
}