#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"

#include<iostream>
#include <filesystem>
#include <vector>
#include <string>
using namespace std;
namespace fs = std::filesystem;
# define CASE_FLIP         0b00000001
# define CASE_MOSAIC       0b00000010
# define CASE_GAUSSIAN     0b00000100
# define CASE_LAPLACIAN    0b00001000
# define CASE_FISHEYE      0b00010000
# define CASE_RESTORATION  0b00100000
# define CASE_ROTATION     0b01000000
# define CASE_SPECIFICATION 0b10000000

void LoadImageFilenames(const std::string& folderPath, std::vector<std::string>& filenames) {
    filenames.clear(); 
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            if (path.ends_with(".jpg") || path.ends_with(".png") || path.ends_with(".bmp")) {
                filenames.push_back(path);
            }
        }
    }
}


void loadCase(int8_t option, GrayImage* img, BitFieldFilter* filter)
{
    if (option & CASE_FLIP) {
        filter->HorizontalFlip(img);
    }
    if (option & CASE_MOSAIC) {
        filter->MosaicFilter(img, 10);
    }
    if (option & CASE_GAUSSIAN) {
        filter->GaussianFilter(img, 5, 1.0);
    }
    if (option & CASE_LAPLACIAN) {
        filter->LaplacianFilter(img);
    }
    if (option & CASE_FISHEYE) {
        filter->FisheyeFilter(img);
    }
    if (option & CASE_RESTORATION) {
        filter->ImageRestoration(img);
    }
    if (option & CASE_ROTATION) {
        filter->ImageRotate(img, 30.0f);
    }
    if (option & CASE_SPECIFICATION) {
        cout << "Image List:" << endl;
        vector<string> filenames;
        LoadImageFilenames("Image-Folder", filenames);
        for (const auto& name : filenames) {
            fs::path p(name);
            cout << p.filename().string() << endl;
        }
        string picname;
        while (true) {
            cout << "Which picture do you want to process (enter the name from image list): ";
            cin >> picname; 
            cin.ignore();
            auto it = std::find_if(filenames.begin(), filenames.end(),
                [&picname](const std::string& fullpath) {
                    return fs::path(fullpath).filename() == picname;
                });
            if (it != filenames.end())  break;
            else cout << "? Picture not found. Please enter a valid name from the list." << endl;
        }
        GrayImage* img2 = new GrayImage();
        img2->LoadImage("Image-Folder/" + picname);
        filter->HistogramSpecification(img, img2);
        delete img2; 
    }
    //img->Display_X_Server();
}

void loadCase(int8_t option, RGBImage* img, BitFieldFilter* filter)
{
    if (option & CASE_FLIP) {
        filter->HorizontalFlip(img);
    }
    if (option & CASE_MOSAIC) {
        filter->MosaicFilter(img, 10);
    }
    if (option & CASE_GAUSSIAN) {
        filter->GaussianFilter(img, 5, 1.0);
    }
    if (option & CASE_LAPLACIAN) {
        filter->LaplacianFilter(img);
    }
    if (option & CASE_FISHEYE) {
        filter->FisheyeFilter(img);
    }
    if (option & CASE_RESTORATION) {
        filter->ImageRestoration(img);
    }
    if (option & CASE_ROTATION) {
        filter->ImageRotate(img, 30.0f);
    }
    if (option & CASE_SPECIFICATION) {
        cout << "Image List:" << endl;
        vector<string> filenames;
        LoadImageFilenames("Image-Folder", filenames);
        for (const auto& name : filenames) {
            fs::path p(name);
            cout << p.filename().string() << endl;
        }
        string picname;
        while (true) {
            cout << "Which picture do you want to process (enter the name from image list): ";
            cin >> picname; 
            cin.ignore();
            auto it = std::find_if(filenames.begin(), filenames.end(),
                [&picname](const std::string& fullpath) {
                    return fs::path(fullpath).filename() == picname;
                });
            if (it != filenames.end())  break;
            else cout << "? Picture not found. Please enter a valid name from the list." << endl;
        }
        RGBImage* img2 = new RGBImage();
        img2->LoadImage("Image-Folder/" + picname);
        filter->HistogramSpecification(img, img2);
        delete img2; 
    }
    //img->Display_X_Server();

}


int main(int argc, char *argv[]){
    
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    //img1->Display_X_Server();
    img1->Display_ASCII();
    delete img1;
    img1 = nullptr;
    
    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    //img2->Display_X_Server();
    img2->Display_ASCII();
    delete img2;
    img2 = nullptr;
    
    cout<<endl;
    cout<<"**************************************************"<<endl;
    cout<<"Demo Bit Field Filter"<<endl;
    cout<<"**************************************************"<<endl;
    cout<<endl;
    
    int fnum;
    while(1){
    
        cout << "Image List:" << endl;
        vector<string> filenames;
        string folder = "Image-Folder"; 
        LoadImageFilenames(folder, filenames);
        for (const auto& name : filenames) {
            fs::path p(name);
            cout << p.filename().string() << endl;
        }
        string picname;
        RGBImage* img1 = new RGBImage();
        
        while (true) { //選擇圖片，找到就退出迴圈
            cout << "Which picture do you want to process (enter the name from image list): ";
            cin >> picname; 
            cin.ignore();
            auto it = std::find_if(filenames.begin(), filenames.end(),
                [&picname](const std::string& fullpath) {
                    return fs::path(fullpath).filename() == picname;
                });
            if (it != filenames.end())  break;
            else {
              cout << "? Picture not found. Please enter a valid name from the list." << endl;
              cout<<endl;
              cout<<"**************************************************"<<endl;
              cout<<endl;
            }
        }
        img1->LoadImage(folder + "/" + picname);
        
        //img1->Display_X_Server();
        BitFieldFilter* filter = new BitFieldFilter();
        cout << "Please enter the number of method you want to use (1~8): ";
        cin >> fnum;
        int fc[fnum] = {0};
        cin.ignore(); // 清除緩衝區的換行符號
        cout<<"Please select the filter method(s): " << endl;
        cout<<"0. HorizontalFlip"<<endl;
        cout<<"1. MosaicFilter"<<endl;
        cout<<"2. GaussianFilter"<<endl;
        cout<<"3. LaplacianFilter"<<endl;
        cout<<"4. FisheyeFilter"<<endl;
        cout<<"5. ImageRestoration"<<endl;
        cout<<"6. ImageRotate"<<endl;
        cout<<"7. HistogramSpecification"<<endl;
        cout<<"Please enter the number: ";
        for(int i = 0; i < fnum; ++i){
            cin >> fc[i];
            cin.ignore(); // 清除緩衝區的換行符號
        }
        int8_t option;
        if(fnum == 1)  option = 1 << fc[0]; 
        else if(fnum == 2)  option = (1 << fc[0]) | (1 << fc[1]); 
        else if(fnum == 3)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]);
        else if(fnum == 4)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]) | (1 << fc[3]);
        else if(fnum == 5)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]) | (1 << fc[3]) | (1 << fc[4]);
        else if(fnum == 6)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]) | (1 << fc[3]) | (1 << fc[4]) | (1 << fc[5]);
        else if(fnum == 7)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]) | (1 << fc[3]) | (1 << fc[4]) | (1 << fc[5]) | (1 << fc[6]);
        else if(fnum == 8)  option = (1 << fc[0]) | (1 << fc[1]) | (1 << fc[2]) | (1 << fc[3]) | (1 << fc[4]) | (1 << fc[5]) | (1 << fc[6]) | (1 << fc[7]);
        else{
            cout<<"Invalid choice."<<endl;
            cout<<endl;
            cout<<"**************************************************"<<endl;
            cout<<endl;
            delete img1;
            img1 = nullptr;
            delete filter;
            filter = nullptr;
            continue; // Skip to the next iteration
        }
        loadCase(option, img1, filter);
        delete img1;
        img1 = nullptr;
        delete filter; 
        filter = nullptr;
        
        int conti;
        cout<<"Continue to Demo Bit Field Filter?"<<endl;
        cout<<"1. Yes"<<endl;
        cout<<"2. No"<<endl;
        cout<<"Please enter the number: ";
        cin>>conti;
        if(conti==1) {
          cout<<endl;
          cout<<"**************************************************"<<endl;
          cout<<endl;
          continue;
        }
        else{
          cout << "Exiting Bit Field Filter Demo." << endl;
          cout<<endl;
          cout<<"**************************************************"<<endl;
          cout<<endl;
          break;
        }
        break;
    }
    
    
    cout<<"**************************************************"<<endl;
    cout<<"Demo Image Encryption"<<endl;
    cout<<"**************************************************"<<endl;
    cout<<endl;
    // encryption and decryption
    Image *img3 = new RGBImage();
    img3->LoadImage("Image-Folder/lena.jpg");
    img3->DumpImage("img2.jpg");
    //img3->Display_X_Server();
    img3->Display_ASCII();
    delete img3;
    img3 = nullptr;
    cout<<endl;
    cout<<"**************************************************"<<endl;
    cout<<endl;
    int choice;
    while(1){
        ImageEncryption* img_ed = new ImageEncryption();
        string password="";
        cout<<"Please select the encryption method:"<<endl;
        cout<<"0. End ImageEncryption Demo"<<endl;
        cout<<"1. LSB encoding"<<endl;
        cout<<"2. XOR encoding"<<endl;
        cout<<"3. Caesar encoding"<<endl;
        cout<<"4. Substitution encoding"<<endl;
        cout<<"Please enter the number: ";
        cin>>choice;
        cin.ignore();
        
        if(choice == 0){
            cout<<endl;
            cout << "Exiting ImageEncryption Demo." << endl;
            cout<<endl;
            cout<<"**************************************************"<<endl;
            cout<<endl;
            delete img_ed; 
            img_ed = nullptr;
            break;
        }
        else if(choice == 1){
            cout << "Demo LSB encoding & decoding." << endl;
            cout<< "Please enter the password: "<<endl;
            getline(cin, password); //getline can get space
            RGBImage* encrypted = img_ed->encode_LSB("Image-Folder/lena.jpg", password);
            
            if (encrypted != nullptr) {
                encrypted->DumpImage("encrypted_lsb.jpg");
                //encrypted->Display_X_Server();
                encrypted->Display_ASCII();
                cout<<endl;
                //decryption
                string p = img_ed->decode_LSB(encrypted);
                if (!p.empty()) {
                    cout << "Decrypted password: " << p << endl;
                } else {
                    cout << "Failed to decrypt the image." << endl;
                }
            } 
            else {
                cout << "Encryption failed." << endl;
            }
            delete encrypted;
            encrypted = nullptr;
        }
        
        else if(choice == 2){
            int cus_flag;
            cout << "Demo XOR encoding & decoding" << endl;
            cout<< "Please enter the password: "<<endl;
            getline(cin, password); //getline can get space
            cout<<"Do you want to use a custom key?"<<endl;
            cout<<"1. Yes"<<endl;
            cout<<"2. No"<<endl;
            cout<<"Please enter the number:";
            cin>> cus_flag;
            cin.ignore();
            if(cus_flag == 1){
                string key;
                cout<<"Please enter the key: "<<endl;
                getline(cin, key); //getline can get space
                RGBImage* encrypted = img_ed->encode_XOR("Image-Folder/lena.jpg", password, key);
                if (encrypted != nullptr) {
                    encrypted->DumpImage("encrypted_xor.jpg");
                    //encrypted->Display_X_Server();
                    encrypted->Display_ASCII();
                    cout<<endl;
                    //decryption
                    string p = img_ed->decode_XOR(encrypted, key);
                    if (!p.empty()) {
                        cout << "Decrypted password: " << p << endl;
                    } else {
                        cout << "Failed to decrypt the image." << endl;
                    }
                } 
                else {
                    cout << "Encryption failed." << endl;
                }
                delete encrypted;
                encrypted = nullptr;
            }
            else if(cus_flag == 2){
                RGBImage* encrypted = img_ed->encode_XOR("Image-Folder/lena.jpg", password);
                if (encrypted != nullptr) {
                    encrypted->DumpImage("encrypted_xor.jpg");
                    //encrypted->Display_X_Server();
                    encrypted->Display_ASCII();
                    cout<<endl;
                    //decryption
                    string p = img_ed->decode_XOR(encrypted);
                    if (!p.empty()) {
                        cout << "Decrypted password: " << p << endl;
                    } else {
                        cout << "Failed to decrypt the image." << endl;
                    }
                } 
                else {
                    cout << "Encryption failed." << endl;
                }
                delete encrypted;
                encrypted = nullptr;
            }
            else{
                cout<<"Invalid choice."<<endl;
            }

        }
        else if(choice == 3){
            cout << "Demo Caesar encoding & decoding" << endl;
            cout<< "Please enter the password: "<<endl;
            getline(cin, password); //getline can get space
            RGBImage* encrypted = img_ed->encode_Caesar("Image-Folder/lena.jpg", password);
            if (encrypted != nullptr) {
                encrypted->DumpImage("encrypted_caesar.jpg");
                //encrypted->Display_X_Server();
                encrypted->Display_ASCII();
                cout<<endl;
                //decryption
                string p = img_ed->decode_Caesar(encrypted);
                if (!p.empty()) {
                    cout << "Decrypted password: " << p << endl;
                } else {
                    cout << "Failed to decrypt the image." << endl;
                }
            } 
            else {
                cout << "Encryption failed." << endl;
            }
            delete encrypted;
            encrypted = nullptr;
        }
        else if(choice == 4){
            cout << "Demo Substitution encoding & decoding" << endl;
            cout<< "Please enter the password: "<<endl;
            getline(cin, password); //getline can get space
            RGBImage* encrypted = img_ed->encode_Subs("Image-Folder/lena.jpg", password);
            if (encrypted != nullptr) {
                encrypted->DumpImage("encrypted_subs.jpg");
                //encrypted->Display_X_Server();
                encrypted->Display_ASCII();
                cout<<endl;
                //decryption
                string p = img_ed->decode_Subs(encrypted);
                if (!p.empty()) {
                    cout << "Decrypted password: " << p << endl;
                } else {
                    cout << "Failed to decrypt the image." << endl;
                }
            } 
            else {
                cout << "Encryption failed." << endl;
            }
            delete encrypted;
            encrypted = nullptr;
        }
        else{
            cout << "Invalid choice." << endl;
            delete img_ed; // Clean up memory
            img_ed = nullptr;
            cout<<endl;
            cout<<"**************************************************"<<endl;
            cout<<endl;
            continue; // Skip to the next iteration
        }
        
        delete img_ed; 
        img_ed = nullptr;
        cout<<endl;
        cout<<"**************************************************"<<endl;
        cout<<endl;
        
    }
    
    return 0;
}