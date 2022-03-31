
#include <iostream>
#include <chrono>
#include <algorithm>
#include <execution>
#include <fstream>

const size_t testWidth = 768;
const size_t testHeight = 576;
const size_t sampleSize = 300;
const int n = 5;


static void FarnebackPolyExpPPstl(const std::vector<float>& src, std::vector<float>& dst)
{
    int width = testWidth;
    int height = testHeight;
    std::vector<float> kbuf (n*6 + 3, 0.23);
    double ig11 = 0.12, ig03 = 0.14, ig33 = 0.13, ig55 = 1.23;

    auto src_ptr = &src[0];
    auto dst_ptr = dst.data();
    std::for_each(std::execution::par_unseq, src.begin(), src.end(),[=](auto &pix){
        int xgOff = n + n*2 +1;
        int xxgOff = xgOff + n*2 + 1;
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

static void FarnebackPolyExpPPstl2(const std::vector<float>& src, std::vector<float>& dst)
{
    int width = testWidth;
    int height = testHeight;
    std::vector<float> kbuf (n*6 + 3, 0.23);
    double ig11 = 0.12, ig03 = 0.14, ig33 = 0.13, ig55 = 1.23;

    auto src_ptr = &src[0];
    auto dst_ptr = dst.data();
    std::for_each(std::execution::par_unseq, src.begin(), src.end(),[=](auto &pix){
        int xgOff = n + n*2 +1;
        int xxgOff = xgOff + n*2 +1;
        float g0 = kbuf[0+n];
        float rBuf;
        float xrBuf;
        float xxrBuf;

        double b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;

        auto index = &pix - src_ptr;
        int x = index % width;
        int y = index / width;

        for( int a = 0; a < 2*n+1; a++){
            int neighX = std::max(x + a-n, 0);
            neighX = std::min(neighX, width-1);
            rBuf = src_ptr[neighX + y * width] * g0;
            xrBuf = 0;
            xxrBuf = 0;
            for(int b = 1; b <= n; b++) {
                int neighY0 = std::max((y - b) * width, 0);
                int neighY1 = std::min((y + b) * width, (height - 1) * width);
                rBuf += (src_ptr[neighX + neighY0] + src_ptr[neighX + neighY1]) * kbuf[b+n];
                xrBuf += (src_ptr[neighX + neighY1] - src_ptr[neighX + neighY0]) * kbuf[b+xgOff];
                xxrBuf += (src_ptr[neighX + neighY0] + src_ptr[neighX + neighY1]) * kbuf[b+xxgOff];
            }
            float g = kbuf[abs(a-n) + n];
            float xg = kbuf[abs(a-n) + xgOff];
            float xxg = kbuf[abs(a-n) + xxgOff];
            if(a == n){
                b1 += rBuf * g;
                b3 += xrBuf * g;
                b5 += xxrBuf * g;
            } else if(a > n){
                b1 += rBuf * g;
                b2 += rBuf * xg;
                b3 += xrBuf * g;
                b4 += rBuf * xxg;
                b5 += xxrBuf * g;
                b6 += xrBuf * xg;
            } else {
                b1 += rBuf * g;
                b2 -= rBuf * xg;
                b3 += xrBuf * g;
                b4 += rBuf * xxg;
                b5 += xxrBuf * g;
                b6 -= xrBuf * xg;
            }
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
    srand(43156844);
    /*
    std::ofstream myFile;
    myFile.open ("example.txt");
    myFile << "Timings\n";
    myFile.close();
    */
    std::cout << "calculating polynomial coefficients for " << sampleSize << " Frames..." << std::endl;
    for (int z = 0; z < sampleSize ; ++z) {
        //create src vector with random sample data
        std::vector<float> src (testHeight * testWidth);
        for(float & i : src){
            i = float(rand() % 255 + 30);
        }
        std::vector<float> dst ((testHeight*testWidth)*5);
        //auto start = std::chrono::steady_clock::now();
        FarnebackPolyExpPPstl2(src, dst);
        //auto end = std::chrono::steady_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::duration<double,std::milli>>(end - start).count();
        /*
        myFile.open ("example.txt",  std::ios_base::app);
        myFile << duration <<"\n";
        myFile.close();
        */
    }
    return 0;
}

