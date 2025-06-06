#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"

int main(int argc, char *argv[]){
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg"); //載入黑白圖片
    img1->DumpImage("img1.jpg"); //導出圖片(jpg/png)
    img1->Display_X_Server(); //彈出視窗顯示圖片
    img1->Display_ASCII(); //終端機顯示圖片
    delete img1;
    img1 = nullptr; //避免懸空指標
    
    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg"); //載入彩色圖片
    img2->DumpImage("img2.jpg"); //導出圖片(jpg/png)
    img2->Display_X_Server(); //彈出視窗顯示圖片
    img2->Display_ASCII(); //終端機顯示圖片
    delete img2;
    img2 = nullptr; //避免懸空指標
    
    // some bit field filter design driven code here

    // some photo mosaic driven code here

    // encryption and decryption --->memory problem
    ImageEncryption* img_ed = new ImageEncryption();
    RGBImage* encrypted = img_ed->encode_LSB("Image-Folder/lena.jpg", "hello123");
    
    if (encrypted != nullptr) {
        encrypted->DumpImage("encrypted.jpg");
        encrypted->Display_X_Server();
        encrypted->Display_ASCII();
        //decryption
        string password = img_ed->decode_LSB(encrypted);
        if (!password.empty()) {
            cout << "Decrypted password: " << password << endl;
        } else {
            cout << "Failed to decrypt the image." << endl;
        }
    } 
    else {
        cout << "Encryption failed." << endl;
    }
    delete encrypted;
    delete img_ed; 
    encrypted = nullptr;
    img_ed = nullptr;
    
    
    
    return 0;
}
