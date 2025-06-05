#include "bit_field_filter.h"
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

void HorizontalFlip(GrayImage &img){
    int h = img.get_h();
    int w = img.get_w();
    int **pixels = img.pixels;;
    for(int = 0; i < h; ++i){
        for(int j = 0;j < w / 2; ++j){ // 以寬度中心為軸左右翻轉
            swap(pixels[i][j], pixels[i][w - j - 1]);
        }
    }
}
void HorizontalFlip(RGBImage &img){
    int h = img.get_h();
    int w = img.get_w();
    int ***pixels = img.pixels;;
    for(int = 0; i < h; ++i){
        for(int j = 0;j < w / 2; ++j){
            for(int k = 0; k<3; k++){
                swap(pixels[i][j][k], pixels[i][w - j - 1][k]);
            }
        }
    }
}

void MosiacFilter(GrayImage &img, int block_size){
    int h = img.get_h();
    int w = img.get_w();
    int **pixels = img.pixels;
    for(int i = 0; i < h; i += block_size){
        for(int j = 0; j < w; j += block_size){
            int sum = 0, count = 0;
            for(int ib = i; ib < min(i+block_size, h); ++ib){
                for(int jb = j; min(j+block_size, w); ++jb){
                    sum += pixels[ib][jb];
                    ++count;
                }
            }
            int avg = sum / count;
            for(int ib = i; ib < min(i+block_size, h); ++ib){
                for(int jb = j; min(j+block_size, w); ++jb){
                    pixels[ib][jb] = avg;
                }
            }
        }
    }
}


void MosiacFilter(RGBImage &img, int block_size){
    int h = img.get_h();
    int w = img.get_w();
    int ***pixels = img.pixels;
    for(int i = 0; i < h; i += block_size){
        for(int j = 0; j < w; j += block_size){
            int sum[3] = {0, 0, 0}, count = 0;
            for(int ib = i; ib < min(i+block_size, h); ++ib){
                for(int jb = j; min(j+block_size, w); ++jb){
                    for(int k = 0; k<3; k++){
                        sum[k] += pixels[ib][jb][k];
                    }
                    ++count;
                }
            }
            for(int ib = i; ib < min(i+block_size, h); ++ib){
                for(int jb = j; min(j+block_size, w); ++jb){
                    for(int k = 0;k<3;k++){
                        pixels[ib][jb][k] = sum[k] / count;
                    }
                }
            }
        }
    }
}

vector<vector<double>> GenerateGaussianKernel(int kernel_size, double sigma) {
    vector<vector<double>> kernel(kernel_size, vector<double>(kernel_size));
    double sum = 0.0;
    int center = kernel_size / 2;
    double twoSigmaSq = 2.0 * sigma * sigma;

    for (int i = 0; i < kernel_size; ++i) {
        for (int j = 0; j < kernel_size; ++j) {
            int x = i - center;
            int y = j - center;
            kernel[i][j] = exp(-(x * x + y * y) / twoSigmaSq) / (M_PI * twoSigmaSq);
            sum += kernel[i][j];
        }
    }
    for (int i = 0; i < kernel_size; ++i)
        for (int j = 0; j < kernel_size; ++j)
            kernel[i][j] /= sum;

    return kernel;
}

void GaussianFilter(GrayImage &img, int kernel_size, double sigma) {
    int h = img.get_h();
    int w = img.get_w();
    int **pixels = img.pixels;
    vector<vector<double>> kernel = GenerateGaussianKernel(kernel_size, sigma);
    int center = kernel_size / 2;
    int **copy = new int*[h];
    for(int i = 0; i < h; ++i) copy[i] = new int[w];

    for(int i = center; i < h-center ; ++i){
        for(int j = center; j< w-center; ++j){
            double sum = 0.0;
            for(int ik = 0; ik < kernel_size; ++ik){
                for(int jk = 0; jk < kernel_size; ++jk){
                    int ni = i + ik - center;
                    int nj = j + jk - center;
                    sum += pixels[ni][nj] * kernel[ik][jk];
                }
            }
            copy[i][j] = min(max(int(round(sum)), 0), 255); // pixel只會界在0~255之間
        }
    }
    for(int i = center; i < h - center; ++i){
        for(int j = center; j < w - center; ++j){
            pixels[i][j] = copy[i][j];
        }
    }
    for(int i = 0; i < h; ++i) delete[] copy[i] ;
    delete[] copy;
}

void GaussianFilter(RGBImage &img, int kernel_size, double sigma) {
    int h = img.get_h();
    int w = img.get_w();
    int ***pixels = img.pixels;
    vector<vector<double>> kernel = GenerateGaussianKernel(kernel_size, sigma);
    int center = kernel_size / 2;
    int ***copy = new int**[h];
    for (int i = 0; i < h; ++i) {
        copy[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            copy[i][j] = new int[3];
            for (int c = 0; c < 3; ++c)
                copy[i][j][c] = 0;
        }
    }

    for(int i = center; i < h-center ; ++i){
        for(int j = center; j< w-center; ++j){
            for(int c = 0; c < 3; ++c){
                double sum = 0.0;
                for(int ik = 0; ik < kernel_size; ++ik){
                    for(int jk = 0; jk < kernel_size; ++jk){
                        int ni = i + ik - center;
                        int nj = j + jk - center;
                        sum += pixels[ni][nj][c] * kernel[ik][jk];
                    }
                }
                copy[i][j][c] = min(max(int(round(sum)), 0), 255); // pixel只會界在0~255之間
            }
        }
    }
    for(int i = center; i < h - center; ++i){
        for(int j = center; j < w - center; ++j){
            for(int c = 0; c < 3; ++c)
                pixels[i][j][c] = copy[i][j][c];
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            delete[] copy[i][j];
        delete[] copy[i];
    }
    delete[] copy;
}

const int LAPLACIAN_KERNEL[3][3] = {
    {0 , -1 , 0},
    {-1, 5 , -1},
    {0 , -1, 0}
}

void LaplacianFilter(GrayImage &img){
    int h = img.get_h();
    int w = img.get_w();
    int **pixels = img.pixels;
    int **copy = new int*[h];
    for(int i = 0; i < h; ++i) copy[i] = new int[w];

    for(int i = 1; i < h-1 ; ++i){
        for(int j = 1; j< w-1; ++j){
            int sum = 0;
            for(int ik = -1; ik <= 1; ++ik){
                for(int jk = -1; jk <= 1; ++jk){
                    sum += pixels[i+ik][j+jk] * LAPLACIAN_KERNEL[ik+1][jk+1];
                }
            }
            copy[i][j] = min(max(sum, 0), 255); // pixel只會界在0~255之間
        }
    }
    for(int i = 1; i < h-1; ++i){
        for(int j = 1; j < w-1; ++j){
            pixels[i][j] = copy[i][j];
        }
    }
    for(int i = 0; i < h; ++i) delete[] copy[i] ;
    delete[] copy;
}

void LaplacianFilter(RGBImage &img){
    int h = img.get_h();
    int w = img.get_w();
    int ***pixels = img.pixels;
    int ***copy = new int**[h];
    for (int i = 0; i < h; ++i) {
        copy[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            copy[i][j] = new int[3];
            for (int c = 0; c < 3; ++c)
                copy[i][j][c] = 0;
        }
    }

    for(int i = 1; i < h-1 ; ++i){
        for(int j = 1; j< w-1; ++j){
            for(int c = 0; c < 3; ++c){
                int sum = 0;
                for(int ik = -1; ik <= 1; ++ik){
                    for(int jk = -1; jk <= 1; ++jk){
                        sum += pixels[i+ik][j+jk][c] * LAPLACIAN_KERNEL[ik+1][jk+1];
                    }
                }
                copy[i][j][c] = min(max(sum, 0), 255); // pixel只會界在0~255之間
            }
        }
    }
    for(int i = 1; i < h-1; ++i){
        for(int j = 1; j < w-1; ++j){
            for(int c = 0; c < 3; ++c)
            pixels[i][j][c] = copy[i][j][c];
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            delete[] copy[i][j];
        delete[] copy[i];
    }
    delete[] copy;
}