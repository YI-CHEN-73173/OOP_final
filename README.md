# Image Processing
## 🔧 編譯方式

### 安裝與編譯
make install   # 安裝第三方函式庫（第一次執行需使用）  
make           # 編譯整個專案

### 執行程式
./Image_Processing

## 🧪 使用說明

**************************************************
### Demo Bit Field Filter
**************************************************
1.啟動後，請選擇要處理的圖片檔名：  
Image List:  
3-2.jpg  
1-2.jpg  
2-2.jpg  
1-1.jpg  
truck.png  
4-1.jpg  
4-2.jpg  
3-1.jpg  
2-1.jpg  
lena.jpg  
Which picture do you want to process (enter the name from image list):  
******************************
2.請輸入要處理幾種方式  
Please enter the number of method you want to use (1~8):
****
3.請輸入要處理的方式（以空格隔開）e.g. 0 1 2 3 4 5 6 7  
Please select the filter method(s):  

0. HorizontalFlip  
1. MosaicFilter  
2. GaussianFilter  
3. LaplacianFilter  
4. FisheyeFilter  
5. ImageRestoration  
6. ImageRotate  
7. HistogramSpecification

Please enter the number:
***
4.若選擇方法7，則需要再選擇一張圖片，輸入方式同1.
***
5.選擇是否要繼續處理其他圖片。選擇1為重複步驟1.到4.；選擇2則結束Demo Bit Field Filter  
Continue to Demo Bit Field Filter?  
1. Yes  
2. No

Please enter the number:
***
**************************************************
### Demo Image Encryption
**************************************************
1.請選擇要使用的加解密方法。選擇0則結束Demo Image Encryption  
Please select the encryption method:  

0. End ImageEncryption Demo  
1. LSB encoding  
2. XOR encoding  
3. Caesar encoding  
4. Substitution encoding
   
Please enter the number:  
***
2.請輸入要加密的字串  
Please enter the password:  
***
3.若選擇方法2，可再選擇是否要自行輸入key。若不自行輸入，則使用預設的key進行加密  
Do you want to use a custom key?  

1. Yes  
2. No
   
Please enter the number:  
***
4.自行輸入key  
Please enter the key:  
***
輸入密碼和所需的key後，程式會加密圖片並可解密回原始密碼，執行結果顯示於畫面或彈出視窗中。

## 🔍 記憶體檢查
make check

## 📁 專案結構
.  
├── Data-Loader  
│   ├── data_loader.cpp  
│   └── data_loader.h  
├── data_loader_demo.cpp  
├── Data_Loader_Example  
├── Image-Folder  
│   ├── 1-1.jpg  
│   ├── 1-2.jpg  
│   ├── 2-1.jpg  
│   ├── 2-2.jpg  
│   ├── 3-1.jpg  
│   ├── 3-2.jpg  
│   ├── 4-1.jpg  
│   ├── 4-2.jpg  
│   ├── lena.jpg  
│   └── truck.png  
├── Image_Processing  
├── inc  
│   ├── bit_field_filter.h  
│   ├── gray_image.h  
│   ├── image_encryption.h  
│   ├── image.h  
│   └── rgb_image.h  
├── LICENSE  
├── main.cpp  
├── Makefile  
├── obj  
│   ├── bit_field_filter.d  
│   ├── bit_field_filter.o  
│   ├── data_loader.d  
│   ├── data_loader.o  
│   ├── gray_image.d  
│   ├── gray_image.o  
│   ├── image.d  
│   ├── image_encryption.d  
│   ├── image_encryption.o  
│   ├── image.o  
│   ├── rgb_image.d  
│   └── rgb_image.o  
├── README.md  
├── scripts  
│   └── clone_env.sh  
├── src  
│   ├── bit_field_filter.cpp  
│   ├── gray_image.cpp  
│   ├── image.cpp  
│   ├── image_encryption.cpp  
│   └── rgb_image.cpp  
└── third-party  
    ├── catimg  
    ├── CImg  
    └── libjpeg  
