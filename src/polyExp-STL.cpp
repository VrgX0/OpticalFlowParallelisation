
#include <iostream>
#include <chrono>
#include <algorithm>
#include <execution>

const size_t testWidth = 10;
const size_t testHeight = 10;
int n = 5;


static void FarnebackPolyExpPPstl(const std::vector<float>& src, std::vector<float>& dst)
{
    int width = testWidth;
    int height = testHeight;
    std::vector<float> kbuf (n*6 + 3, 0.23);
    double ig11 = 0.12, ig03 = 0.14, ig33 = 0.13, ig55 = 1.23;

    auto src_ptr = src.data();
    auto dst_ptr = dst.data();
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
    std::vector<float> src (testHeight * testWidth);
    for(float & i : src){
        i = 5.f;
    }
    std::vector<float> dst ((testHeight*testWidth)*5);
    std::cout << "begin" << std::endl;
    FarnebackPolyExpPPstl(src, dst);
    std::cout << "end" << std::endl;
    return 0;
}

