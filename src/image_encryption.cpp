#include<iostream>
#include<string>
#include<vector>
#include "image_encryption.h"
using namespace std;

//-------------------------------------------------LSB
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
//--------------------------------------------------------XOR
RGBImage* ImageEncryption::encode_XOR(string filename, string password) { //預設key
    string XOR_key="OOP";
    return encode_XOR(filename, password, XOR_key);
}
RGBImage* ImageEncryption::encode_XOR(string filename, string password, string key) {
    if (password.empty() || key.empty()) {
        cout << "Password or key is empty." << endl;
        return nullptr;
    }

    string cipher = "";
    int pass_len = password.length(); // password長度
    int key_len = key.length(); // key長度

    for (int i = 0; i < pass_len; i++) {
        cipher += password[i] ^ key[i % key_len];  // 循環使用key並XOR
    }

    return encode_LSB(filename, cipher);  // 將加密後的密碼藏入圖片中
}
string ImageEncryption::decode_XOR(RGBImage* image) { //預設key
    string XOR_key="OOP";
    return decode_XOR(image, XOR_key);
}
string ImageEncryption::decode_XOR(RGBImage* image, string key) {
    if (image == nullptr || image->get_w() == 0 || image->get_h() == 0) {
        cout << "Invalid image for decoding." << endl;
        return "";
    }

    string cipher = decode_LSB(image); // 從圖片中解密出密碼
    if (cipher.empty()) {
        cout << "Failed to decode the image." << endl;
        return "";
    }

    string pass = "";
    int cipher_len = cipher.length();
    int key_len = key.length();

    for (int i = 0; i < cipher_len; i++) {
        pass += cipher[i] ^ key[i % key_len]; // 循環使用key並XOR
    }

    return pass; // 返回解密後的密碼
}
//--------------------------------------------------------Caesar
RGBImage* ImageEncryption::encode_Caesar(string filename, string password) {
    if (password.empty()) {
        cout << "Password is empty." << endl;
        return nullptr;
    }

    string cipher = "";
    for (char ch : password) {
        int ascii_val = static_cast<int>(ch);
        int encrypted_val = (ascii_val + C_shift) % 256; // 限制在0~255的範圍
        cipher += static_cast<char>(encrypted_val);
    }

    return encode_LSB(filename, cipher); // 將加密後的密碼藏入圖片中
}
string ImageEncryption::decode_Caesar(RGBImage* image) {
    if (image == nullptr || image->get_w() == 0 || image->get_h() == 0) {
        cout << "Invalid image for decoding." << endl;
        return "";
    }

    string cipher = decode_LSB(image); // 從圖片中解密出密碼
    if (cipher.empty()) {
        cout << "Failed to decode the image." << endl;
        return "";
    }

    string pass = "";
    for (char ch : cipher) {
        int ascii_val = static_cast<int>(ch);
        int plain = (ascii_val - C_shift + 256) % 256;
        pass += static_cast<char>(plain);
    }

    return pass; // 返回解密後的密碼
}
//--------------------------------------------------------Substitution
void ImageEncryption::generate_table() {
    int range = 256;
    vector<int> values;

    for (int i = 0; i < range; ++i)
        values.push_back(i);

    srand(time(NULL));
    for (int i = range - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(values[i], values[j]);
    }

    subs.clear();
    reverse_subs.clear();
    for (int i = 0; i < range; ++i) {
        subs[i] = values[i];
        reverse_subs[values[i]] = i;
    }
}
RGBImage* ImageEncryption::encode_Subs(string filename, string password) {
    if (password.empty()) {
        cout << "Password is empty." << endl;
        return nullptr;
    }

    generate_table(); // 每次加密時產生新的對照表

    string cipher = "";
    for (unsigned char ch : password) {
        cipher += static_cast<unsigned char>(subs[ch]);
    }

    return encode_LSB(filename, cipher);
}
string ImageEncryption::decode_Subs(RGBImage* image) {
    if (!image || image->get_w() == 0 || image->get_h() == 0) {
        cout << "Invalid image." << endl;
        return "";
    }

    string cipher = decode_LSB(image);
    if (cipher.empty()) {
        cout << "Decoded string is empty." << endl;
        return "";
    }

    string pass = "";
    for (unsigned char ch : cipher) {
        if (reverse_subs.find(ch) != reverse_subs.end()) {
            pass += static_cast<unsigned char>(reverse_subs[ch]);
        } else {
            cout << "Character '" << ch << "' not found in reverse table." << endl;
            return "";
        }
    }
    return pass;
}