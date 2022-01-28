
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
    auto cmd = "py testPlotter.py";


    system(cmd);

    fs::current_path(originalPath);
    return 0;
}

