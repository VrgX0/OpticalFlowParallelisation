#########################
# C++17 implementation
#########################
add_executable(polyExp_stl polyExp-STL.cpp)
add_executable(DenseFlow denseFlow.cpp)

target_link_libraries(polyExp_stl TBB::tbb ${OpenCV_LIBS} )
target_link_libraries(DenseFlow TBB::tbb ${OpenCV_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Sample DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# gnu implementation of cpp17 parallel stl at least requires Intel's TBB
#add_library(simpleTest STATIC cpp-impl.cpp)
#target_link_libraries(cpp-impl PRIVATE TBB::tbb)
#target_include_directories(cpp-impl PUBLIC include)
#target_link_libraries(testit-cpp PUBLIC cpp-impl)
if (CMAKE_CXX_COMPILER_ID STREQUAL "NVHPC")
    message("Using Nvidia's HPC SDK Compiler for C++ implementation, enabling stdpar GPU offloading")
    # enable GPU offloading for C++17 parallel STL calls
    #target_compile_options(DenseFlow PUBLIC "-stdpar")
    #target_link_options(DenseFlow PUBLIC "-stdpar")
    target_compile_options(polyExp_stl PUBLIC "-stdpar")
    target_link_options(polyExp_stl PUBLIC "-stdpar")
endif()