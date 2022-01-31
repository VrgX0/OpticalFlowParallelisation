#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "optflowgf.cpp"
#include <filesystem>
#include <chrono>

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

#if defined(_WIN32)
#define VIDEO "../Sample/vtest_000/vtest_%03d.png"
#else
#define VIDEO "Sample/vtest_000/vtest_%03d.png"
#endif

int main()
{
    cout << "start optflow" << endl;
    //VideoCapture capture(R"(D:\MPI-Sintel-complete\training\clean\bamboo_1\frame_%04d.png)");
    VideoCapture capture((fs::current_path() / VIDEO).generic_string());

    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    //initialise first frame
    Mat frame1, prvs;
    capture >> frame1;
    //convert into Grayscale picture
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    auto startLoop = chrono::high_resolution_clock::now();
    while(true){
        //initialize second frame
        Mat frame2, next;
        capture >> frame2;
        //check if sequence ended
        if (frame2.empty())
            break;
        //convert into Grayscale picture
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        auto start = chrono::high_resolution_clock::now();
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        auto end = chrono::high_resolution_clock::now();
        // visualization
        Mat flow_parts[2];
        //split flow into multiple
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        //calculate magnitude and angles
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));

        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        imshow("frame2", bgr);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
        cout << chrono::duration_cast<chrono::duration<double, milli>>(end - start).count() << endl;
    }
    auto endLoop = chrono::high_resolution_clock::now();
    //cout << chrono::duration_cast<chrono::duration<double, milli>>(endLoop - startLoop).count() << endl;
}

