
#include <iostream>
#include <chrono>
#include <algorithm>
#include <execution>
#include <opencv2/opencv.hpp>

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
const size_t testWidth = 760;
const size_t testHeight = 580;



static void FarnebackPolyExpPPstl(cv::Mat& src, cv::Mat& dst, int n, double sigma)
{
    int width = testWidth;
    int height = testHeight;
    std::vector<float> kbuf (n*6 + 3);
    double ig11 = 0.12, ig03 = 0.14, ig33 = 0.13, ig55 = 1.23;

    //dst.create(testSize,testSize,CV_32FC(5));
    auto src_ptr = src.ptr<float>(0);
    auto dst_ptr = dst.ptr<float>(0);
    std::for_each(std::execution::par_unseq, src_ptr,src_ptr + (width * height),[=](auto &pix){
        int xgOff = n + n*2 +1;
        int xxgOff = xgOff +n*2;
        float g0 = kbuf[0+n];
        std::vector<float> rBuf((2 * n + 1)*3,0.f);
        int offset = 2*n+1;

        auto index = &pix - src_ptr;
        int x = index % width;
        int y = index / width;

        for( int a = 0; a < 2*n+1; a++){
            int neighX = std::max(x + a-n, 0);
            neighX = std::min(neighX, width-1);
            rBuf[a] = src_ptr[neighX + y * width] * g0;
            for(int b = 1; b <= n; b++) {
                int neighY0 = std::max((y - b) * width, 0);
                int neighY1 = std::min((y + b) * width, (height - 1) * width);
                rBuf[a] += (src_ptr[neighX + neighY0] + src_ptr[neighX + neighY1]) * kbuf[b+n];
                rBuf[a + offset] += (src_ptr[neighX + neighY1] - src_ptr[neighX + neighY0]) * kbuf[b+xgOff];
                rBuf[a + 2 * offset] += (src_ptr[neighX + neighY0] + src_ptr[neighX + neighY1]) * kbuf[b+xxgOff];
            }
        }

        double b1 = rBuf[n]*g0, b2 = 0, b3 = rBuf[n + offset]*g0, b4 = 0, b5 = rBuf[n + 2*offset]*g0, b6 = 0;
        for( int a = 1; a <= n; a++){

            b1 += (rBuf[n+a] + rBuf[n-a]) * kbuf[a+n];
            b2 += (rBuf[n+a] - rBuf[n-a]) * kbuf[a+xgOff];
            b4 += (rBuf[n+a] + rBuf[n-a]) * kbuf[a+xxgOff];
            b3 += (rBuf[n+a+offset] + rBuf[n-a+offset]) * kbuf[a+n];
            b6 += (rBuf[n+a+offset] - rBuf[n-a+offset]) * kbuf[a+xgOff];
            b5 += (rBuf[n+a+offset*2] + rBuf[n-a+offset*2]) * kbuf[a+n];
        }

        int pixel = x+y*width;
        dst_ptr[pixel*5+1] = (float)(b2*ig11);
        dst_ptr[pixel*5] = (float)(b3*ig11);
        dst_ptr[pixel*5+3] = (float)(b1*ig03 + b4*ig33);
        dst_ptr[pixel*5+2] = (float)(b1*ig03 + b5*ig33);
        dst_ptr[pixel*5+4] = (float)(b6*ig55);
    });
}


int main() {

    //auto src = new cv::Mat(testSize, testSize, CV_32FC1);
    //auto dst = new cv::Mat(testSize, testSize, CV_32FC(5));
    //src.create(testSize, testSize, CV_32FC1);
    /*
    for (int i = 0; i < testSize*testSize; ++i) {
        src->at<float>(i) = 5.f;
    }
    */
    std::vector<float> src (testWidth*testHeight);
    //original src Mat contains Greyscale values between 0 and 255
    for(float & i : src){
        int randNum = rand()%(254-1 + 1) + 1;
        i = (float)randNum;
    }

    cv::Mat srcMat = cv::Mat(testHeight,testWidth,CV_32FC1,src.data());
    std::vector<float> dst ((testWidth*testHeight)*5);
    cv::Mat dstMat = cv::Mat(testHeight,testWidth,CV_32FC(5),dst.data());
    auto begin = std::chrono::high_resolution_clock::now();
    FarnebackPolyExpPPstl(srcMat, dstMat, 5, 1.2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double,std::milli>>(end - begin).count();
    std::cout << "Time for " << testWidth << "x" << testHeight <<" frame is " << duration << " ms" << std::endl;

    return 0;
}

