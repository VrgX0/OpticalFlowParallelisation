
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

double getDuration (high_resolution_clock::time_point start, high_resolution_clock::time_point end){
    return duration_cast<duration<double, std::milli>>(end - start).count();
}

void print_results(high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime){
    std::cout << "Time " << duration_cast<duration<double, std::milli>>(endTime - startTime).count()
                                                                     << " ms" << std::endl;
}

const size_t testSize = 1000000000;

int main() {
    const auto start = high_resolution_clock::now();
    std::random_device rd;
    // generate randoms
    std::vector<double> doubles(testSize);
    for (auto &d: doubles) {
        d = static_cast<double>(rd());
    }
    std::vector<double>doublesB(doubles);
    std::vector<double> doublesC(testSize);

    const auto firstStepTimeBegin = high_resolution_clock::now();
    std::transform( doublesB.begin(), doublesB.end(), doublesC.begin(),
                    [](double v){return v*v;} );
    const auto firstStepTimeEnd = high_resolution_clock::now();
    print_results(firstStepTimeBegin, firstStepTimeEnd);

    const auto secondStepTimeBegin= high_resolution_clock::now();
    std::transform(std::execution::seq, doublesB.begin(), doublesB.end(), doublesC.begin(),
                   [](double v){return v*v;} );
    const auto secondStepTimeEnd = high_resolution_clock::now();
    print_results(secondStepTimeBegin, secondStepTimeEnd);

    const auto thirdStepTimeBegin= high_resolution_clock::now();
    std::transform(std::execution::par, doublesB.begin(), doublesB.end(), doublesC.begin(),
                   [](double v){return v*v;} );
    const auto thirdStepTimeEnd = high_resolution_clock::now();
    print_results(thirdStepTimeBegin, thirdStepTimeEnd);

    const auto fourthStepTimeBegin= high_resolution_clock::now();
    std::transform(std::execution::par_unseq, doublesB.begin(), doublesB.end(), doublesC.begin(),
                   [](double v){return v*v;} );
    const auto fourthStepTimeEnd = high_resolution_clock::now();
    print_results(fourthStepTimeBegin, fourthStepTimeEnd);

    fs::path originalPath = fs::current_path();
    fs::current_path(originalPath/R"(..\..\..\src)");

    double durationToOne = getDuration(start, firstStepTimeBegin);
    double durationStepOne = getDuration(start,firstStepTimeEnd);
    double durationToTwo = getDuration(start, secondStepTimeBegin);
    double durationStepTwo = getDuration(start,secondStepTimeEnd);
    double durationToThree = getDuration(start, thirdStepTimeBegin);
    double durationStepThree = getDuration(start, thirdStepTimeEnd);
    double durationToFour = getDuration(start, fourthStepTimeBegin);
    double durationStepFour = getDuration(start, fourthStepTimeEnd);

    std::ofstream myFile;
    myFile.open ("example.txt");
    myFile << "durationToOne:" << durationToOne << "\n" << "durationStepOne:" << durationStepOne << "\n"
           << "durationToTwo:" << durationToTwo << "\n" << "durationStepTwo:" << durationStepTwo << "\n"
           << "durationToThree:" << durationToThree << "\n" << "durationStepThree:" << durationStepThree << "\n"
           << "durationToFour:" << durationToFour << "\n" << "durationStepFour:" << durationStepFour << "\n";
    myFile.close();

    int n = 5;
    std::vector<float> rowBuf(11 + n * 2, 0.f), xRowBuf(11 + n * 2, 0.f), xxRowBuf(11 + n * 2, 0.f);
    std::vector<float> gb(2*n+1), xgb(2*n+1), xxgb(2*n+1);
    std::vector<int> test (11);
    std::iota(test.begin(), test.end(),0);
    auto mainExPo = std::execution::par;
    std::vector<float> b1(11), b2(11), b3(11), b4(11), b5(11), b6(11);
    std::for_each(mainExPo,test.begin(), test.end(),
                  [n, &rowBuf, &gb, &b1, &xRowBuf, &b3, &xxRowBuf, &b5, &b2, &xgb, &b6, &xxgb, &b4, mainExPo](auto x){
                      int w = 2 * n + 1;
                      std::vector<float>vec (w);
                      //from row with normal gb
                      std::transform(mainExPo, rowBuf.begin()+x, rowBuf.begin() + w + x, gb.begin(), vec.begin(), [](auto &a, auto&b){
                          return a*b;
                      });
                      b1[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                      //from xRow with normal gb
                      std::transform(mainExPo, xRowBuf.begin()+x, xRowBuf.begin() + w + x, gb.begin(), vec.begin(), [](auto &a, auto&b){
                          return a*b;
                      });
                      b3[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                      //from xxRow with normal gb
                      std::transform(mainExPo, xxRowBuf.begin()+x, xxRowBuf.begin() + w + x, gb.begin(), vec.begin(), [](auto &a, auto&b){
                          return a*b;
                      });
                      b5[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                      //from xRow with xgb[n] = 0
                      std::transform(mainExPo, rowBuf.begin()+x, rowBuf.begin() + w + x, xgb.begin(), vec.begin(), [](auto &a, auto&b){
                          return a*b;
                      });
                      b2[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                      std::transform(mainExPo, xRowBuf.begin()+x, xRowBuf.begin() + w + x, xgb.begin(), vec.begin(),[](auto &a, auto&b){
                          return a*b;
                      });
                      b6[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                      std::transform(mainExPo, rowBuf.begin()+x, rowBuf.begin()+w+x, xxgb.begin(), vec.begin(),[](auto &a, auto&b){
                          return a*b;
                      });
                      b4[x] = std::accumulate(vec.begin(), vec.end(), 0.f);
                  });


    auto cmd = "py testPlotter.py";


    system(cmd);

    fs::current_path(originalPath);
    return 0;
}

