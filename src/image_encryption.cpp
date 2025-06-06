#include<iostream>
#include<string>
#include<vector>
#include "image_encryption.h"
using namespace std;

RGBImage* ImageEncryption::encode_LSB(string filename, string password) {
    RGBImage* image = new RGBImage();
    image->LoadImage(filename);
    int width = image->get_w(); //取得圖片寬度
    int height = image->get_h(); //取得圖片高度
    int*** pix = image->get_pixels();  // 拿pixels矩陣
    if (pix == nullptr) {
        cout << "Failed to get pixels from image." << endl;
        delete image;
        return nullptr;
    }

    if (password.empty()) { //檢查輸入的字串是否為空
        cout << "Password is empty." << endl;
        delete image;
        return nullptr;
    }
    int length = password.length(); //字串長度
    int bit_num = 16 + length * 8; //需要的長度
    int total_bit = width * height * 3; //總共的長度
    //cout<<"length: "<<length<<", bit_num: "<<bit_num<<", total_bit: "<<total_bit<<endl;
    vector<int> bits; //用來存字串中每個字元的二進制
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
    for (int i = 0; i < length; ++i) {
        char word = password[i];
        int ascii_val = static_cast<int>(word); //將字元轉成ASCII值
        for (int j = 7; j >= 0; j--) { //從左邊到右邊
            bits.push_back((ascii_val >> j) & 1);   
        }
    }

    //藏內容
    int bit_index = 0;
    for (int y = 0; y < height && bit_index < bit_num; y++) {
        for (int x = 0; x < width && bit_index < bit_num; x++) {
            for (int c = 0; c < 3 && bit_index < bit_num; c++) {
                int original = pix[y][x][c];
                int modified = (original & 0xFE) | bits[bit_index++];
                image->set_pixels(x, y, c, modified);
            }
        }
    }
    
    bits.clear(); //清空
    bits.shrink_to_fit(); // 強制釋放記憶體
    return image; //回傳加密後的圖片
}

string ImageEncryption::decode_LSB(RGBImage* image) {
    if (image == nullptr || image->get_w() == 0 || image->get_h() == 0) {
        cout << "Invalid image for decoding." << endl;
        return "";
    }

    int width = image->get_w(); //取得圖片寬度
    int height = image->get_h(); //取得圖片高度
    int total_bit = width * height * 3;

    // 檢查解碼長度（16 bits）
    if (total_bit < 16) {
        cout << "Not enough bits to decode length." << endl;
        return "";
    }

    int*** pix = image->get_pixels(); // 取出 pixels
    if (pix == nullptr) {
        cout << "Failed to get pixels from image." << endl;
        return "";
    }

    vector<int> bits; //用來存字串中每個字元的二進制
    //先全部取出
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) {
                bits.push_back(pix[y][x][c] & 1);
            }
        }
    }
    int bit_index = 0;
    int length = 0;
    for (int i = 0; i < 16; ++i) {
        length = (length << 1) | bits[bit_index];
        bit_index++;
    }
    //cout<<length<<endl;
    // 長度檢查
    if (length >= 65536 || bit_index + length * 8 > total_bit) { //unsigned vs. signed
        cout << "Invalid or corrupted decoded length." << endl;
        bits.clear(); //清空
        bits.shrink_to_fit(); // 強制釋放記憶體
        return "";
    }

    // 解密
    string password = "";
    for (int i = 0; i < length; ++i) {
        int ch = 0;
        for (int j = 0; j < 8; ++j) {
            ch = (ch << 1) | bits[bit_index];
            bit_index++;
        }
        //cout<<"ch="<<ch<<endl;
        password += static_cast<char>(ch); //把二進制轉成字元
    }
    bits.clear();
    bits.shrink_to_fit(); // 強制釋放記憶體
    return password;
}