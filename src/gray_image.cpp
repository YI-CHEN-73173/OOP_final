#include<iostream>
#include<string>
#include "gray_image.h"
using namespace std;

GrayImage::GrayImage() : Image() {
    pixels = nullptr;
}
GrayImage::GrayImage(int width, int height) : Image(width, height) {
    pixels = new int*[height];
    for (int i = 0; i < height; ++i) {
        pixels[i] = new int[width];
    }
}
GrayImage::GrayImage(int width, int height, int **p) : Image(width, height) {
    pixels = new int*[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = p[i][j];
        }
    }
}
GrayImage::GrayImage(GrayImage &gi): Image(gi.get_w(), gi.get_h()) {
    pixels = new int*[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = gi.pixels[i][j];
        }
    }
}
GrayImage::~GrayImage() {
    if(pixels != nullptr){
        for (int i = 0; i < h; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    pixels = nullptr;
}
GrayImage& GrayImage::operator=(GrayImage &gi) {
    if (this == &gi) return *this;
    if(pixels != nullptr) {
        for (int i = 0; i < h; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }

    h = gi.get_h();
    w = gi.get_w();
    pixels = new int*[h];
    for (int i = 0; i < h; ++i) {
        pixels[i] = new int[w];
        for (int j = 0; j < w; ++j) {
            pixels[i][j] = gi.pixels[i][j];
        }
    }
    return *this;
}
bool GrayImage::LoadImage(string filename) {//加載圖片
    if (pixels != nullptr) {
        for (int i = 0; i < h; ++i) {
            delete[] pixels[i];
        }
        delete[] pixels;
    }
    pixels = data_loader.Load_Gray(filename, &w, &h);
    return pixels != nullptr; //成功加載圖片回傳1，否則回傳0
}
void GrayImage::DumpImage(string filename) { //輸出圖片(jpg/png)
    data_loader.Dump_Gray(w, h, pixels, filename); //給定檔名輸出
}
void GrayImage::Display_X_Server() { //彈出視窗顯示圖片
    data_loader.Display_Gray_X_Server(w, h, pixels); 
}
void GrayImage::Display_ASCII() { //終端機顯示圖片
    data_loader.Display_Gray_ASCII(w, h, pixels);
}
int **GrayImage::get_pixels() { //拿pixels
    return pixels;
}
void GrayImage::set_pixels(int w, int h, int p) { //設定pixels
    if(pixels == nullptr) {
        pixels = new int*[h];
        for (int i = 0; i < h; ++i) {
            pixels[i] = new int[w];
        }
    }
    pixels[h][w] = p; //設定像素值
}
