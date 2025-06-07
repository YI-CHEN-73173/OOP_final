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

    vector<unsigned char> bytes(password.begin(), password.end());
    int length = bytes.size();  // UTF-8 的 byte 數
    int bit_num = 16 + length * 8; //需要的長度
    int total_bit = width * height * 3; //總共的長度
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
    for (unsigned char byte : bytes) {
        for (int j = 7; j >= 0; j--) {
            bits.push_back((byte >> j) & 1);
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
    // 長度檢查
    if (length >= 65536 || bit_index + length * 8 > total_bit) { //unsigned vs. signed
        cout << "Invalid or corrupted decoded length." << endl;
        bits.clear(); //清空
        bits.shrink_to_fit(); // 強制釋放記憶體
        return "";
    }
    vector<unsigned char> bytes;
    for (int i = 0; i < length; ++i) {
        unsigned char ch = 0;
        for (int j = 0; j < 8; ++j) {
            ch = (ch << 1) | bits[bit_index++];
        }
        bytes.push_back(ch);
    }
    string password(bytes.begin(), bytes.end());

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
    vector<unsigned char> pass_bytes(password.begin(), password.end());
    vector<unsigned char> key_bytes(key.begin(), key.end());
    vector<unsigned char> cipher_bytes;

    for (size_t i = 0; i < pass_bytes.size(); ++i) {
        cipher_bytes.push_back(pass_bytes[i] ^ key_bytes[i % key_bytes.size()]); //XOR
    }

    string cipher(cipher_bytes.begin(), cipher_bytes.end()); // 用 string 儲存，加進 LSB
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
    vector<unsigned char> cipher_bytes(cipher.begin(), cipher.end());
    vector<unsigned char> key_bytes(key.begin(), key.end());
    vector<unsigned char> pass;

    for (size_t i = 0; i < cipher_bytes.size(); ++i) {
        pass.push_back(cipher_bytes[i] ^ key_bytes[i % key_bytes.size()]); //XOR
    }
    return string(pass.begin(), pass.end());
}
//--------------------------------------------------------Caesar
RGBImage* ImageEncryption::encode_Caesar(string filename, string password) {
    if (password.empty()) {
        cout << "Password is empty." << endl;
        return nullptr;
    }
    vector<unsigned char> pass_bytes(password.begin(), password.end());
    vector<unsigned char> cipher_bytes;

    for (unsigned char ch : pass_bytes) {
        cipher_bytes.push_back((ch + C_shift) % 256); // 平移
    }

    string cipher(cipher_bytes.begin(), cipher_bytes.end());
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
    vector<unsigned char> cipher_bytes(cipher.begin(), cipher.end());
    vector<unsigned char> pass_bytes;

    for (unsigned char ch : cipher_bytes) {
        pass_bytes.push_back((ch - C_shift + 256) % 256); // 平移
    }

    return string(pass_bytes.begin(), pass_bytes.end());
}
//--------------------------------------------------------Substitution
void ImageEncryption::generate_table() {
    int range = 256;
    vector<int> values;

    for (int i = 0; i < range; ++i)
        values.push_back(i);

    srand(time(NULL)); // 隨機種子
    for (int i = range - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(values[i], values[j]); // 交換位置
    }

    subs.clear();
    reverse_subs.clear();
    for (int i = 0; i < range; ++i) {
        subs[i] = values[i]; // 對照表
        reverse_subs[values[i]] = i; // 反向對照表
    }
    return;
}
RGBImage* ImageEncryption::encode_Subs(string filename, string password) {
    if (password.empty()) {
        cout << "Password is empty." << endl;
        return nullptr;
    }

    generate_table(); // 每次加密時產生新的對照表

    string cipher = "";
    for (unsigned char ch : password) {
        cipher += static_cast<unsigned char>(subs[ch]); // 用對照表加密
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
            pass += static_cast<unsigned char>(reverse_subs[ch]); // 用反向對照表解密
        } else {
            cout << "Character '" << ch << "' not found in reverse table." << endl;
            return "";
        }
    }
    return pass;
}