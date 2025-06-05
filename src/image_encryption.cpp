#include<iostream>
#include<string>
#include<deque>
#include "image_encryption.h"
using namespace std;

RGBImage* ImageEncryption::encode_LSB(string filename, string password) {
    RGBImage* image = new RGBImage();
    if (!image->LoadImage(filename)) { //檢查讀圖是否成功
        cout << "Failed to load image: " << filename << endl;
        delete image;
        return nullptr;
    }

    int width = image->get_w(); //取得圖片寬度
    int height = image->get_h(); //取得圖片高度

    if (password.empty()) { //檢查輸入的字串是否為空
        cout << "Password is empty." << endl;
        delete image;
        return nullptr;
    }
    int length = password.length(); //字串長度
    int bit_num = 16 + length * 8; //需要的長度
    int total_bit = width * height * 3; //總共的長度
    deque<int> bits; //用來存字串中每個字元的二進制
    //檢查字串長度是否超過限制
    if (length >= 65536 || bit_num > total_bit) {
        cout << "Password is too long." << endl;
        delete image;
        return nullptr;
    }
    //先存字串長度的二進制 -> 65535位內
    for (int i = 15; i >= 0; i--) { 
        bits.push_back((length >> i) & 1);
    }
    //存字串內容的二進制
    for (char word : password) {
        for (int j = 7; j >= 0; j--) { //從左邊到右邊
            bits.push_back((word >> j) & 1); // 取出第i個字元的第j個 bit
        }
    }

    int*** pix = image->get_pixels();  // 拿pixels矩陣
    //藏內容
    for (int y = 0; y < height && !bits.empty(); y++) {
        for (int x = 0; x < width && !bits.empty(); x++) {
            for (int c = 0; c < 3 && !bits.empty(); c++) {
                int original = pix[y][x][c];
                int modified = (original & 0xFE) | bits.front();  // 改最後一個bit
                bits.pop_front();
                image->set_pixels(x, y, c, modified);  // 寫回圖片
            }
        }
    }
    return image; //回傳加密後的圖片
}

string ImageEncryption::decode_LSB(RGBImage* image) {
    if (image == nullptr || image->get_w() == 0 || image->get_h() == 0) {
        cout << "Invalid image for decoding." << endl;
        return "";
    }

    int width = image->get_w(); //取得圖片寬度
    int height = image->get_h(); //取得圖片高度
    int*** pix = image->get_pixels(); // 取出 pixels

    deque<int> bits; //用來存字串中每個字元的二進制
    //先全部取出
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                bits.push_back(pix[y][x][c] & 1);
            }
        }
    }
    // 檢查解碼長度（16 bits）
    if (bits.size() < 16) {
        cout << "Not enough bits to decode length." << endl;
        return "";
    }

    int length = 0;
    for (int i = 0; i < 16; ++i) {
        length = (length << 1) | bits.front();
        bits.pop_front();
    }

    // 長度檢查
    if (length >= 65536 || bits.size() < static_cast<size_t>(length) * 8) { //unsigned vs. signed
        cout << "Invalid or corrupted encoded length." << endl;
        return "";
    }

    // 解碼密碼
    string password = "";
    for (int i = 0; i < length; ++i) {
        int ch = 0;
        for (int j = 0; j < 8; ++j) {
            ch = (ch << 1) | bits.front();
            bits.pop_front();
        }
        password += static_cast<char>(ch); //把二進制轉成字元
    }

    return password;
}