# Bachelor thesis: Modernes paralleles C++ für Hardwarebeschleuniger
  Complete thesis can be found in /docs.
  
  Sample video and Sample code sourced from: 
  https://docs.opencv.org/4.5.5/d4/dee/tutorial_optical_flow.html
  and 
  https://github.com/opencv/opencv
  
  ## Prerequisites

  * C++ compiler with C++17 support for cpu only code
  * nvc++ compiler from Nvidia's HPC SDK for stdpar
  * OpenCV libraires (tested with OpenCV 4.5.4)
  
  ## Instructions
  
  For GPU code compile the code in a build directory with:
  ```
  cmake -DCMAKE_CXX_COMPILER=nvc++ -DCMAKE_BUILD_TYPE=Release ..
  ```
