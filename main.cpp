#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"

int main(int argc, char *argv[]){
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    img1->Display_ASCII();
    delete img1;
    img1 = nullptr;
    
    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    img2->Display_ASCII();
    delete img2;
    img2 = nullptr;
    
    // some bit field filter design driven code here

    // some photo mosaic driven code here

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
            cout << "Exiting ImageEncryption Demo." << endl;
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
                encrypted->Display_X_Server();
                encrypted->Display_ASCII();
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
                    encrypted->Display_X_Server();
                    encrypted->Display_ASCII();
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
                    encrypted->Display_X_Server();
                    encrypted->Display_ASCII();
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
                encrypted->Display_X_Server();
                encrypted->Display_ASCII();
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
                encrypted->Display_X_Server();
                encrypted->Display_ASCII();
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
