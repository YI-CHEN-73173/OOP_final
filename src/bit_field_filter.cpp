#include "bit_field_filter.h"
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

GrayImage* BitFieldFilter::HorizontalFlip(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int **pixels = img->GetPixels();
    
    GrayImage* result = new GrayImage(*img);
    int** newPixels = result->GetPixels();
    
    for(int i = 0; i < h; ++i){
        for(int j = 0;j < w ; ++j){ 
            newPixels[i][j] = pixels[i][w - j - 1];
        }
    }
    return result;
}
RGBImage* BitFieldFilter::HorizontalFlip(RGBImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();

    RGBImage* result = new RGBImage(*img);
    int*** newPixels = result->GetPixels();

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int k = 0; k < 3; ++k) {
                newPixels[i][j][k] = pixels[i][w - j - 1][k];
            }
        }
    }
    return result;
}

GrayImage* BitFieldFilter::MosaicFilter(GrayImage* img, int block_size){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();
    GrayImage* result = new GrayImage(*img);
    int** newPixels = result->GetPixels();

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
                    newPixels[ib][jb] = avg;
                }
            }
        }
    }
    return result;
}


RGBImage* BitFieldFilter::MosaicFilter(RGBImage* img, int block_size) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();
    RGBImage* result = new RGBImage(*img);
    int*** newPixels = result->GetPixels();

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
                        newPixels[ib][jb][k] = sum[k] / count;
                    }
                }
            }
        }
    }
    return result;
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

GrayImage* BitFieldFilter::GaussianFilter(GrayImage* img, int kernel_size, double sigma) {
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();
    std::vector<std::vector<double>> kernel = GenerateGaussianKernel(kernel_size, sigma);
    int center = kernel_size / 2;
    GrayImage* result = new GrayImage(*img);
    int** resultPixels = result->GetPixels();

    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            double sum = 0.0;
            for (int ik = 0; ik < kernel_size; ++ik) {
                for (int jk = 0; jk < kernel_size; ++jk) {
                    int ni = i + ik - center;
                    int nj = j + jk - center;
                    sum += pixels[ni][nj] * kernel[ik][jk];
                }
            }
            resultPixels[i][j] = std::min(std::max(int(std::round(sum)), 0), 255);
        }
    }
    return result;
}


RGBImage* BitFieldFilter::GaussianFilter(RGBImage* img, int kernel_size, double sigma) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();
    std::vector<std::vector<double>> kernel = GenerateGaussianKernel(kernel_size, sigma);
    int center = kernel_size / 2;

    RGBImage* result = new RGBImage(*img);
    int*** resultPixels = result->GetPixels();

    for (int i = center; i < h - center; ++i) {
        for (int j = center; j < w - center; ++j) {
            for (int c = 0; c < 3; ++c) {
                double sum = 0.0;
                for (int ik = 0; ik < kernel_size; ++ik) {
                    for (int jk = 0; jk < kernel_size; ++jk) {
                        int ni = i + ik - center;
                        int nj = j + jk - center;
                        sum += pixels[ni][nj][c] * kernel[ik][jk];
                    }
                }
                resultPixels[i][j][c] = std::min(std::max(int(std::round(sum)), 0), 255);
            }
        }
    }
    return result;
}


const int LAPLACIAN_KERNEL[3][3] = {
    {0 , -1 , 0},
    {-1, 5 , -1},
    {0 , -1, 0}
};

GrayImage* BitFieldFilter::LaplacianFilter(GrayImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();

    GrayImage* result = new GrayImage(*img);
    int** resultPixels = result->GetPixels();

    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            int sum = 0;
            for (int ik = -1; ik <= 1; ++ik) {
                for (int jk = -1; jk <= 1; ++jk) {
                    sum += pixels[i + ik][j + jk] * LAPLACIAN_KERNEL[ik + 1][jk + 1];
                }
            }
            resultPixels[i][j] = std::min(std::max(sum, 0), 255); // 限制在 0~255
        }
    }
    return result;
}


RGBImage* BitFieldFilter::LaplacianFilter(RGBImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();

    RGBImage* result = new RGBImage(*img);
    int*** resultPixels = result->GetPixels();

    for (int i = 1; i < h - 1; ++i) {
        for (int j = 1; j < w - 1; ++j) {
            for (int c = 0; c < 3; ++c) {
                int sum = 0;
                for (int ik = -1; ik <= 1; ++ik) {
                    for (int jk = -1; jk <= 1; ++jk) {
                        sum += pixels[i + ik][j + jk][c] * LAPLACIAN_KERNEL[ik + 1][jk + 1];
                    }
                }
                resultPixels[i][j][c] = std::min(std::max(sum, 0), 255);
            }
        }
    }
    return result;
}

GrayImage* BitFieldFilter::FisheyeFilter(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();

    GrayImage* result = new GrayImage(*img);
    int** resultPixels = result->GetPixels();
    
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
                    resultPixels[i][j] = pixels[srcXi][srcYi];
                }
                else{
                    resultPixels[i][j] = 0;
                }
            }
            else{
                resultPixels[i][j] = 0;
            }
        }
    }
    return result;
}
RGBImage* BitFieldFilter::FisheyeFilter(RGBImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();

    RGBImage* result = new RGBImage(*img);
    int*** newPixels = result->GetPixels();

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
                        newPixels[i][j][k] = pixels[srcYi][srcXi][k];
                    }
                } else {
                    for (int k = 0; k < 3; ++k) {
                        newPixels[i][j][k] = 0; 
                    }
                }
            } else {
                for (int k = 0; k < 3; ++k) {
                    newPixels[i][j][k] = 0;
                }
            }
        }
    }
    return result;
}

GrayImage* BitFieldFilter::ImageRestoration(GrayImage* img){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();

    GrayImage* result = new GrayImage(*img);
    int** newPixels = result->GetPixels();
    
    long long sum = 0;
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            sum += pixels[i][j];
        }
    }
    float avg = static_cast<float>(sum) / (h*w);
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            int val = pixels[i][j];
            int enhanced = static_cast<int>((val - avg) * 1.2f + avg);
            if (enhanced < 0) enhanced = 0;
            if (enhanced > 255) enhanced = 255;
            newPixels[i][j] = enhanced;
        }
    }
    return result;
}

RGBImage* BitFieldFilter::ImageRestoration(RGBImage* img) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();

    RGBImage* result = new RGBImage(*img);
    int*** newPixels = result->GetPixels();

    for (int c = 0; c < 3; ++c) { // 對 R、G、B 通道各自做處理
        long long sum = 0;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                sum += pixels[i][j][c];
            }
        }
        float avg = static_cast<float>(sum) / (h * w);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                int val = pixels[i][j][c];
                int enhanced = static_cast<int>((val - avg) * 1.2f + avg);
                if (enhanced < 0) enhanced = 0;
                if (enhanced > 255) enhanced = 255;
                newPixels[i][j][c] = enhanced;
            }
        }
    }
    return result;
}


GrayImage* BitFieldFilter::ImageRotate(GrayImage* img, float angle_degrees){
    int h = img->get_h();
    int w = img->get_w();
    int** pixels = img->GetPixels();

    GrayImage* result = new GrayImage(*img);
    int** newPixels = result->GetPixels();
    
    float angle_rad = angle_degrees * M_PI / 180.0f;
    float cos_theta = std::cos(angle_rad);
    float sin_theta = std::sin(angle_rad);
    int cx = w / 2;
    int cy = h / 2;
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            float x = cos_theta * (j - cx) + sin_theta * (i - cy) + cx;
            float y = -sin_theta * (j - cx) + cos_theta * (i - cy) + cy;
            int x0 = static_cast<int>(std::round(x));
            int y0 = static_cast<int>(std::round(y));
            if(x0 >= 0 && x0 < w && y0 >= 0 && y0 < h){
                newPixels[i][j] = pixels[y0][x0];
            }
            else{
                newPixels[i][j] = 0; // 超出邊界填黑
            }
        }
    }
    return result;
    
}
RGBImage* BitFieldFilter::ImageRotate(RGBImage* img, float angle_degrees) {
    int h = img->get_h();
    int w = img->get_w();
    int*** pixels = img->GetPixels();

    RGBImage* result = new RGBImage(*img);
    int*** newPixels = result->GetPixels();

    float angle_rad = angle_degrees * M_PI / 180.0f;
    float cos_theta = std::cos(angle_rad);
    float sin_theta = std::sin(angle_rad);
    int cx = w / 2;
    int cy = h / 2;
    for (int i = 0; i < h; ++i){
        for (int j = 0; j < w; ++j){
            float x = cos_theta * (j - cx) + sin_theta * (i - cy) + cx;
            float y = -sin_theta * (j - cx) + cos_theta * (i - cy) + cy;
            int x0 = static_cast<int>(std::round(x));
            int y0 = static_cast<int>(std::round(y));
            if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h){
                for (int c = 0; c < 3; ++c){
                    newPixels[i][j][c] = pixels[y0][x0][c];
                }
            } 
            else{
                for (int c = 0; c < 3; ++c){
                    newPixels[i][j][c] = 0; // 超出邊界填黑
                }
            }
        }
    }
    return result;
}

GrayImage* BitFieldFilter::HistogramSpecification(GrayImage* source, GrayImage* reference){
    int h = source->get_h();
    int w = source->get_w();
    int** src_pixels = source->GetPixels();
    //int** ref_pixels = reference->GetPixels();

    GrayImage* result = new GrayImage(*source);
    int** newPixels = result->GetPixels();

    int src_hist[256] = {0};
    int ref_hist[256] = {0};
    
    for(int i = 0; i < h; ++i)
        for(int j = 0; j < w; ++j)
            src_hist[src_pixels[i][j]]++;
            
    int h_ref = reference->get_h();
    int w_ref = reference->get_w();
    for(int i = 0; i < h_ref; ++i)
        for(int j = 0; j < w_ref; ++j)
            src_hist[src_pixels[i][j]]++;
            
    float src_cdf[256] = {0};//normalized cdf
    float ref_cdf[256] = {0};
    int total_src = h * w;
    int total_ref = h_ref * w_ref;
    src_cdf[0] = (float)src_hist[0] / total_src;
    ref_cdf[0] = (float)ref_hist[0] / total_ref;
    
    for (int i = 1; i < 256; ++i){
        src_cdf[i] = src_cdf[i - 1] + (float)src_hist[i] / total_src;
        ref_cdf[i] = ref_cdf[i - 1] + (float)ref_hist[i] / total_ref;
    }
    //mapping
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
            newPixels[i][j] = mapping[src_pixels[i][j]];

    return result;
}

RGBImage* BitFieldFilter::HistogramSpecification(RGBImage* source, RGBImage* reference) {
    int h = source->get_h();
    int w = source->get_w();
    int*** src_pixels = source->GetPixels();
    int*** ref_pixels = reference->GetPixels();

    RGBImage* result = new RGBImage(*source);
    int*** newPixels = result->GetPixels();
    int src_hist[3][256] = {0}; // 0: R, 1: G, 2: B
    int ref_hist[3][256] = {0};

    int h_ref = reference->get_h();
    int w_ref = reference->get_w();

    for (int i = 0; i < h; ++i) // histogram
        for (int j = 0; j < w; ++j) {
            src_hist[0][src_pixels[i][j][0]]++; // R
            src_hist[1][src_pixels[i][j][1]]++; // G
            src_hist[2][src_pixels[i][j][2]]++; // B
        }

    for (int i = 0; i < h_ref; ++i)
        for (int j = 0; j < w_ref; ++j) {
            ref_hist[0][ref_pixels[i][j][0]]++; // R
            ref_hist[1][ref_pixels[i][j][1]]++; // G
            ref_hist[2][ref_pixels[i][j][2]]++; // B
        }

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
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            newPixels[i][j][0] = mapping[0][src_pixels[i][j][0]]; // R
            newPixels[i][j][1] = mapping[1][src_pixels[i][j][1]]; // G
            newPixels[i][j][2] = mapping[2][src_pixels[i][j][2]]; // B
        }
    }
    return result;
}





























