
#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <random>
#include <execution>
#include <filesystem>
#include "string"
#include <fstream>

namespace fs = std::filesystem;

using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
/*
double getDuration (high_resolution_clock::time_point start, high_resolution_clock::time_point end){
    return duration_cast<duration<double, std::milli>>(end - start).count();
}

void print_results(high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime){
    std::cout << "Time " << duration_cast<duration<double, std::milli>>(endTime - startTime).count()
                                                                     << " ms" << std::endl;
}
*/
const size_t testSize = 100;

static void FarnebackPolyExpPPstl(float *src, float* dst, int n, double sigma )
{
    int width = testSize;
    int height = testSize;
    std::vector<float> kbuf(n*6 + 3);
    float* g = kbuf.data() + n;
    float* xg = g + n*2 + 1;
    float* xxg = xg + n*2 + 1;
    double ig11 = 0.3, ig03 = 0.2, ig33 = 0.1, ig55 = 0.4;
    std::fill(kbuf.begin(),kbuf.end(),0.12);
    std::for_each(std::execution::par_unseq, src,src + (width * height),[=](auto &pix){

        float g0 = g[0];
        std::vector<float> rBuf((2 * n + 1)*3, 0.f);
        int offset = 2*n+1;

        auto index = &pix - src;
        int x = index % width;
        int y = index / width;

        for( int a = 0; a < 2*n+1; a++){
            int neighX = std::max(x + a-n, 0);
            neighX = std::min(neighX, width-1);
            rBuf[a] = src[neighX + y * width] * g0;
            for(int b = 1; b <= n; b++) {
                int neighY0 = std::max((y - b) * width, 0);
                int neighY1 = std::min((y + b) * width, (height - 1) * width);
                rBuf[a] += (src[neighX + neighY0] + src[neighX + neighY1]) * g[b];
                rBuf[a + offset] += (src[neighX + neighY1] - src[neighX + neighY0]) * xg[b];
                rBuf[a + 2 * offset] += (src[neighX + neighY0] + src[neighX + neighY1]) * xxg[b];
            }
        }

        double b1 = rBuf[n]*g0, b2 = 0, b3 = rBuf[n + offset]*g0, b4 = 0, b5 = rBuf[n + 2*offset]*g0, b6 = 0;
        for( int a = 1; a <= n; a++){
            b1 += (rBuf[n+a] + rBuf[n-a]) * g[a];
            b2 += (rBuf[n+a] - rBuf[n-a]) * xg[a];
            b4 += (rBuf[n+a] + rBuf[n-a]) * xxg[a];
            b3 += (rBuf[n+a+offset] + rBuf[n-a+offset]) * g[a];
            b6 += (rBuf[n+a+offset] - rBuf[n-a+offset]) * xg[a];
            b5 += (rBuf[n+a+offset*2] + rBuf[n-a+offset*2]) * g[a];
        }

        int pixel = x+y*width;
        dst[pixel*5+1] = (float)(b2*ig11);
        dst[pixel*5] = (float)(b3*ig11);
        dst[pixel*5+3] = (float)(b1*ig03 + b4*ig33);
        dst[pixel*5+2] = (float)(b1*ig03 + b5*ig33);
        dst[pixel*5+4] = (float)(b6*ig55);
    });
}


int main() {

    std::vector<float>src (testSize*testSize);
    for(float & i : src){
        i = 5.f;
    }
    float* src_ptr = src.data();
    std::vector<float>dst ((testSize*testSize)*5);
    float* dst_ptr = dst.data();
    FarnebackPolyExpPPstl(src_ptr, dst_ptr, 5, 2);
    return 0;
}

