cmake_minimum_required(VERSION 3.21)

project(BA_Thesis)
set(CMAKE_CXX_STANDARD 17)

find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_subdirectory(src)