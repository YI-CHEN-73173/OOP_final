## 📁 專案結構
inc/ # 標頭檔 (.h)
src/ # 實作檔 (.cpp)
obj/ # 編譯後的物件檔 (*.o)
Image-Folder/ # 測試圖片（如 lena.jpg）
main.cpp # 主程式
Makefile # 編譯與建構設定

## 🔧 編譯方式

專案透過 `Makefile` 編譯，包含兩個目標：

- `Image_Processing`：主程式
- `Data_Loader_Example`：資料讀取範例

### 安裝與編譯
make install   # 安裝第三方函式庫（第一次執行需使用）
make           # 編譯整個專案

### 執行程式
./Image_Processing

### 🧪 使用說明
啟動後，請依提示選擇加密方式：
Please select the encryption method:
0. End ImageEncryption Demo
1. LSB encoding
2. XOR encoding
3. Caesar encoding
4. Substitution encoding
輸入密碼後，程式會加密圖片並可解密回原始密碼，執行結果顯示於畫面或圖片中。

### 🔍 記憶體檢查
使用 Valgrind 進行記憶體檢測：
make check