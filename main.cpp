#include "image.h"
#include "gray_image.h"
#include "rgb_image.h"
#include "image_encryption.h"
#include "bit_field_filter.h"

#include<iostream>
using namespace std;
# define CASE_ONE   0b00000001
# define CASE_TWO   0b00000010
# define CASE_THREE 0b00000100
# define CASE_FOUR  0b00001000

void loadCase(int8_t option)
{
    if(option & CASE_ONE) 
        cout << "Case 1 detected" << endl;
    if(option & CASE_TWO) 
        cout << "Case 2 detected" << endl;
    if(option & CASE_THREE)
        cout << "Case 3 detected" << endl;  
    if(option & CASE_FOUR)      
        cout << "Case 4 detected" << endl;
}

int main(int argc, char *argv[]){
    Image *img1 = new GrayImage();
    img1->LoadImage("Image-Folder/lena.jpg");
    img1->DumpImage("img1.jpg");
    img1->Display_X_Server();
    img1->Display_ASCII();
    

    Image *img2 = new RGBImage();
    img2->LoadImage("Image-Folder/lena.jpg");
    img2->DumpImage("img2.jpg");
    img2->Display_X_Server();
    img2->Display_ASCII();
    
    uint8_t option = 0b00001001;
    cout << "test1:\n";
    loadCase(option);
    
    cout << "test2:\n";
    loadCase(CASE_ONE | CASE_TWO);

    cout << "test3:\n";
    loadCase(CASE_ONE | CASE_TWO | CASE_THREE | CASE_FOUR);
    // some bit field filter design driven code here

    // some photo mosaic driven code here

    // more ...

    delete img1;
    delete img2;
    return 0;
}
