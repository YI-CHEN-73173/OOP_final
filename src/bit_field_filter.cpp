#include "bit_field_filter.h"
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

void BitFieldFilter::HorizontalFlip(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int **pixels = img->get_pixels();

    for(int i = 0; i < h; ++i){
        for(int j = 0;j < w/2 ; ++j){ 
            
            swap(pixels[i][j], pixels[i][w - j - 1]);
        }
    }
    return;
}
void BitFieldFilter::HorizontalFlip(RGBImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w/2; ++j) {
            for (int k = 0; k < 3; ++k) {
                
                swap(pixels[i][j][k], pixels[i][w - j - 1][k]);
            }
        }
    }
    return;
}

void BitFieldFilter::MosaicFilter(GrayImage* img, int block_size){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();
    
    for (int i = 0; i < h; i += block_size) {
        for (int j = 0; j < w; j += block_size) {
            int sum = 0, count = 0;
            for (int ib = i; ib < std::min(i + block_size, h); ++ib) {
                for (int jb = j; jb < std::min(j + block_size, w); ++jb) {
                    sum += pixels[ib][jb];
                    ++count;
                }
            }
            int avg = sum / count;
            for (int ib = i; ib < std::min(i + block_size, h); ++ib) {
                for (int jb = j; jb < std::min(j + block_size, w); ++jb) {
                    pixels[ib][jb] = avg;
                }
            }
        }
    }
    return ;
}


void BitFieldFilter::MosaicFilter(RGBImage* img, int block_size) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();

    for (int i = 0; i < h; i += block_size) {
        for (int j = 0; j < w; j += block_size) {
            int sum[3] = {0, 0, 0}, count = 0;
            for (int ib = i; ib < std::min(i + block_size, h); ++ib) {
                for (int jb = j; jb < std::min(j + block_size, w); ++jb) {
                    for (int k = 0; k < 3; ++k) {
                        sum[k] += pixels[ib][jb][k];
                    }
                    ++count;
                }
            }
            for (int ib = i; ib < std::min(i + block_size, h); ++ib) {
                for (int jb = j; jb < std::min(j + block_size, w); ++jb) {
                    for (int k = 0; k < 3; ++k) {
                        pixels[ib][jb][k] = sum[k] / count;
                    }
                }
            }
        }
    }
    return;
}

double* GenerateGaussianKernelArray(int kernel_size, double sigma) {
    int size = kernel_size * kernel_size;
    double* kernel = new double[size];
    double sum = 0.0;
    int center = kernel_size / 2;
    double twoSigmaSq = 2.0 * sigma * sigma;

    for (int i = 0; i < kernel_size; ++i) {
        for (int j = 0; j < kernel_size; ++j) {
            int x = i - center;
            int y = j - center;
            double val = exp(-(x * x + y * y) / twoSigmaSq) / (M_PI * twoSigmaSq);
            kernel[i * kernel_size + j] = val;
            sum += val;
        }
    }
    for (int i = 0; i < size; ++i) kernel[i] /= sum;
    return kernel;  
}
void BitFieldFilter::GaussianFilter(GrayImage* img, int kernel_size, double sigma) {
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();
    double* kernel = GenerateGaussianKernelArray(kernel_size, sigma);
    int center = kernel_size / 2;
    vector<vector<int>> temp(h, vector<int>(w, 0));
    
    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            double sum = 0.0;
            for (int ik = 0; ik < kernel_size; ++ik) {
                for (int jk = 0; jk < kernel_size; ++jk) {
                    int ni = i + ik - center;
                    int nj = j + jk - center;
                    sum += pixels[ni][nj] * kernel[ik * kernel_size + jk];
                }
            }
            temp[i][j] = std::min(std::max(int(std::round(sum)), 0), 255);
        }
    }
    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            pixels[i][j] = temp[i][j];
        }
    }
    delete kernel;
    return;
}


void BitFieldFilter::GaussianFilter(RGBImage* img, int kernel_size, double sigma) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();
    double* kernel = GenerateGaussianKernelArray(kernel_size, sigma);
    int center = kernel_size / 2;

    int*** temp = new int**[h];
    for (int i = 0; i < h; ++i) {
        temp[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            temp[i][j] = new int[3]();  // 初始化為 0
        }
    }
    
    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            for (int c = 0; c < 3; ++c) {
                double sum = 0.0;
                for (int ik = 0; ik < kernel_size; ++ik) {
                    for (int jk = 0; jk < kernel_size; ++jk) {
                        int ni = i + ik - center;
                        int nj = j + jk - center;
                        sum += pixels[ni][nj][c] * kernel[ik * kernel_size + jk];
                    }
                }
                temp[i][j][c] = std::min(std::max(int(std::round(sum)), 0), 255);
            }
        }
    }
    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            for (int c = 0; c < 3; ++c) {
                pixels[i][j][c] = temp[i][j][c];
            }
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            delete[] temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;
    delete[] kernel;
    return;
}


const int LAPLACIAN_KERNEL[3][3] = {
    {0 , -1 , 0},
    {-1, 5 , -1},
    {0 , -1, 0}
};

void BitFieldFilter::LaplacianFilter(GrayImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();

    int** temp = new int*[h];
    for (int i = 0; i < h; ++i) {
        temp[i] = new int[w];
        std::fill(temp[i], temp[i] + w, 0);
    }
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int sum = 0;
            for (int ik = -1; ik <= 1; ++ik) {
                for (int jk = -1; jk <= 1; ++jk) {
                    sum += pixels[i + ik][j + jk] * LAPLACIAN_KERNEL[ik + 1][jk + 1];
                }
            }
            temp[i][j] = std::min(std::max(sum, 0), 255);
        }
    }
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            pixels[i][j] = temp[i][j];
        }
    }
    for (int i = 0; i < h; ++i) {
        delete[] temp[i];
    }
    delete[] temp;
    return ;
}


void BitFieldFilter::LaplacianFilter(RGBImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();

    int*** temp = new int**[h];
    for (int i = 0; i < h; ++i) {
        temp[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            temp[i][j] = new int[3]{0, 0, 0};
        }
    }
    
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            for (int c = 0; c < 3; ++c) {
                int sum = 0;
                for (int ik = -1; ik <= 1; ++ik) {
                    for (int jk = -1; jk <= 1; ++jk) {
                        sum += pixels[i + ik][j + jk][c] * LAPLACIAN_KERNEL[ik + 1][jk + 1];
                    }
                }
                temp[i][j][c] = std::min(std::max(sum, 0), 255);
            }
        }
    }
    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            for (int c = 0; c < 3; ++c) {
                pixels[i][j][c] = temp[i][j][c];
            }
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            delete[] temp[i][j];
        }
        delete[] temp[i];
    }
    delete[] temp;
    return ;
}

void BitFieldFilter::FisheyeFilter(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();

    int** temp = new int*[h];
    for (int i = 0; i < h; ++i) {
        temp[i] = new int[w];
        std::fill(temp[i], temp[i] + w, 0);
    }
    
    float cx = w / 2.0;
    float cy = h / 2.0;
    float maxRadius = sqrt(cx * cx + cy * cy);
    
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            //以中心為原點的相對座標
            float dx = j - cx;
            float dy = i - cy;
            float radius = sqrt(dx * dx + dy * dy);
            float r = radius / maxRadius;
            float factor = 1.0; // 加強扭曲程度(0.5~1.5)
            if(r < 1.0){
                float newR = r * r;
                float scale = (r == 0) ? 1.0 : (newR / r) * factor;
                float srcX = cx + dx * scale;
                float srcY = cy + dy * scale;
                int srcXi = std::round(srcX);
                int srcYi = std::round(srcY);
                
                if(srcXi >= 0 && srcXi < w && srcYi >= 0 && srcYi < h){
                    temp[i][j] = pixels[srcYi][srcXi];
                }
                else{
                    temp[i][j] = 0;
                }
            }
            else{
                temp[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            pixels[i][j] = temp[i][j];
    for (int i = 0; i < h; ++i)
        delete[] temp[i];
    delete[] temp;
    return ;
}
void BitFieldFilter::FisheyeFilter(RGBImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();

    int*** temp = new int**[h];
    for (int i = 0; i < h; ++i) {
        temp[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            temp[i][j] = new int[3]{0, 0, 0};
        }
    }

    float cx = w / 2.0;
    float cy = h / 2.0;
    float maxRadius = sqrt(cx * cx + cy * cy);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            float dx = j - cx;
            float dy = i - cy;
            float radius = sqrt(dx * dx + dy * dy);
            float r = radius / maxRadius;
            float factor = 1.0;
            if (r < 1.0) {
                float newR = r * r;
                float scale = (r == 0) ? 1.0 : (newR / r) * factor;
                float srcX = cx + dx * scale;
                float srcY = cy + dy * scale;
                int srcXi = std::round(srcX);
                int srcYi = std::round(srcY);

                if (srcXi >= 0 && srcXi < w && srcYi >= 0 && srcYi < h) {
                    for (int k = 0; k < 3; ++k) {
                        temp[i][j][k] = pixels[srcYi][srcXi][k];
                    }
                } else {
                    for (int k = 0; k < 3; ++k) {
                        temp[i][j][k] = 0; 
                    }
                }
            } else {
                for (int k = 0; k < 3; ++k) { 
                    temp[i][j][k] = 0;
                }
            }
        }
    }
    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            for (int k = 0; k < 3; ++k)
                pixels[i][j][k] = temp[i][j][k];
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            delete[] temp[i][j];
        delete[] temp[i];
    }
    delete[] temp;
    return;
}

void BitFieldFilter::ImageRestoration(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();
    
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            int val = pixels[i][j];
            int enhanced = static_cast<int>(val * 1.5f);
            if (enhanced < 0) enhanced = 0;
            if (enhanced > 255) enhanced = 255;
            pixels[i][j] = enhanced;
        }
    }
    return;
}

void BitFieldFilter::ImageRestoration(RGBImage* img) {
    if (!img) return;
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();

    for (int c = 0; c < 3; ++c) { // 對 R、G、B 通道各自做處理
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int val = pixels[i][j][c];
                int enhanced = static_cast<int>(val * 1.5f);
                if (enhanced < 0) enhanced = 0;
                if (enhanced > 255) enhanced = 255;
                pixels[i][j][c] = enhanced;
            }
        }
    }
    return;
}

void BitFieldFilter::ImageRotate(GrayImage* img, float angle_degrees){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->get_pixels();

    int** backup = new int*[h];
    for (int i = 0; i < h; ++i) {
        backup[i] = new int[w];
        std::copy(pixels[i], pixels[i] + w, backup[i]);
    }

    float angle_rad = angle_degrees * M_PI / 180.0f;
    float cos_theta = std::cos(angle_rad);
    float sin_theta = std::sin(angle_rad);
    int cx = w / 2;
    int cy = h / 2;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            float x = cos_theta * (j - cx) + sin_theta * (i - cy) + cx;
            float y = -sin_theta * (j - cx) + cos_theta * (i - cy) + cy;
            int x0 = static_cast<int>(std::round(x));
            int y0 = static_cast<int>(std::round(y));
            pixels[i][j] = (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h) ? backup[y0][x0] : 0;
        }
    }

    for (int i = 0; i < h; ++i)
        delete[] backup[i];
    delete[] backup;
    return;
    
}
void BitFieldFilter::ImageRotate(RGBImage* img, float angle_degrees) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->get_pixels();
    
    int*** backup = new int**[h];
    for (int i = 0; i < h; ++i) {
        backup[i] = new int*[w];
        for (int j = 0; j < w; ++j) {
            backup[i][j] = new int[3];
            for (int c = 0; c < 3; ++c) {
                backup[i][j][c] = pixels[i][j][c];
            }
        }
    }

    float angle_rad = angle_degrees * M_PI / 180.0f;
    float cos_theta = std::cos(angle_rad);
    float sin_theta = std::sin(angle_rad);
    int cx = w / 2;
    int cy = h / 2;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            float x = cos_theta * (j - cx) + sin_theta * (i - cy) + cx;
            float y = -sin_theta * (j - cx) + cos_theta * (i - cy) + cy;
            int x0 = static_cast<int>(std::round(x));
            int y0 = static_cast<int>(std::round(y));
            if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h) {
                for (int c = 0; c < 3; ++c) {
                    pixels[i][j][c] = backup[y0][x0][c];
                }
            } else {
                for (int c = 0; c < 3; ++c) {
                    pixels[i][j][c] = 0;
                }
            }
        }
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j)
            delete[] backup[i][j];
        delete[] backup[i];
    }
    delete[] backup;
    return;
}

void BitFieldFilter::HistogramSpecification(GrayImage* source, GrayImage* reference){
    int h = source->get_h();
    int w = source->get_w();
    int** src_pixels = source->get_pixels();
    int** ref_pixels = reference->get_pixels();
    int src_hist[256] = {0};
    int ref_hist[256] = {0};
    
    for(int i = 0; i < h; ++i)
        for(int j = 0; j < w; ++j)
            src_hist[src_pixels[i][j]]++;

    int h_ref = reference->get_h();
    int w_ref = reference->get_w();
    for(int i = 0; i < h_ref; ++i)
        for(int j = 0; j < w_ref; ++j)
            ref_hist[ref_pixels[i][j]]++;

    float src_cdf[256] = {0};
    float ref_cdf[256] = {0};
    int total_src = h * w;
    int total_ref = h_ref * w_ref;

    src_cdf[0] = (float)src_hist[0] / total_src;
    ref_cdf[0] = (float)ref_hist[0] / total_ref;

    for (int i = 1; i < 256; ++i){
        src_cdf[i] = src_cdf[i - 1] + (float)src_hist[i] / total_src;
        ref_cdf[i] = ref_cdf[i - 1] + (float)ref_hist[i] / total_ref;
    }

    unsigned char mapping[256];
    for (int i = 0; i < 256; ++i) {
        float min_diff = 1.0f;
        int best_match = 0;
        for (int j = 0; j < 256; ++j) {
            float diff = std::abs(src_cdf[i] - ref_cdf[j]);
            if (diff < min_diff) {
                min_diff = diff;
                best_match = j;
            }
        }
        mapping[i] = static_cast<unsigned char>(best_match);
    }

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            src_pixels[i][j] = mapping[src_pixels[i][j]];
    return;
}

void BitFieldFilter::HistogramSpecification(RGBImage* source, RGBImage* reference) {
    int h = source->get_h();
    int w = source->get_w();
    int*** src_pixels = source->get_pixels();
    int*** ref_pixels = reference->get_pixels();
    int src_hist[3][256] = {0};
    int ref_hist[3][256] = {0};

    int h_ref = reference->get_h();
    int w_ref = reference->get_w();

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            for (int c = 0; c < 3; ++c)
                src_hist[c][src_pixels[i][j][c]]++;

    for (int i = 0; i < h_ref; ++i)
        for (int j = 0; j < w_ref; ++j)
            for (int c = 0; c < 3; ++c)
                ref_hist[c][ref_pixels[i][j][c]]++;

    float src_cdf[3][256] = {0};
    float ref_cdf[3][256] = {0};
    int total_src = h * w;
    int total_ref = h_ref * w_ref;

    for (int c = 0; c < 3; ++c) {
        src_cdf[c][0] = (float)src_hist[c][0] / total_src;
        ref_cdf[c][0] = (float)ref_hist[c][0] / total_ref;
        for (int i = 1; i < 256; ++i) {
            src_cdf[c][i] = src_cdf[c][i - 1] + (float)src_hist[c][i] / total_src;
            ref_cdf[c][i] = ref_cdf[c][i - 1] + (float)ref_hist[c][i] / total_ref;
        }
    }

    unsigned char mapping[3][256];
    for (int c = 0; c < 3; ++c) {
        for (int i = 0; i < 256; ++i) {
            float min_diff = 1.0f;
            int best_match = 0;
            for (int j = 0; j < 256; ++j) {
                float diff = std::abs(src_cdf[c][i] - ref_cdf[c][j]);
                if (diff < min_diff) {
                    min_diff = diff;
                    best_match = j;
                }
            }
            mapping[c][i] = static_cast<unsigned char>(best_match);
        }
    }

    for (int i = 0; i < h; ++i)
        for (int j = 0; j < w; ++j)
            for (int c = 0; c < 3; ++c)
                src_pixels[i][j][c] = mapping[c][src_pixels[i][j][c]];
    return;
}