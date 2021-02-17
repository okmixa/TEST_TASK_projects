cmake_minimum_required(VERSION 2.8.12)
project(sklm_test)

add_compile_options(-std=c++17 -O4 -pthread)

include_directories(test_cpp)
set(SRC test_cpp/Image.cpp
        test_cpp/ImageProcessing.cpp
        test_cpp/Test.cpp
        test_cpp/Timer.cpp
        test_cpp/main.cpp
)


find_package (Threads)
add_executable(sklm_test ${SRC})
target_link_libraries (sklm_test ${CMAKE_THREAD_LIBS_INIT})
