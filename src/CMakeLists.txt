#########################
# C++17 implementation
#########################
add_executable(test simpleTest.cpp)
add_executable(DenseFlow denseFlow.cpp optflowgf.cpp)
target_link_libraries(DenseFlow ${OpenCV_LIBS})
# gnu implementation of cpp17 parallel stl at least requires Intel's TBB
#add_library(simpleTest STATIC cpp-impl.cpp)
#target_link_libraries(cpp-impl PRIVATE TBB::tbb)
#target_include_directories(cpp-impl PUBLIC include)
#target_link_libraries(testit-cpp PUBLIC cpp-impl)
if (CMAKE_CXX_COMPILER_ID STREQUAL "NVHPC")
    message("Using Nvidia's HPC SDK Compiler for C++ implementation, enabling stdpar GPU offloading")
    # enable GPU offloading for C++17 parallel STL calls
    target_compile_options(cpp-impl PUBLIC "-stdpar")
    target_link_options(cpp-impl PUBLIC "-stdpar")
endif()