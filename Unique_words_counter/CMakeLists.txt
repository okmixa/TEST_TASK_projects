cmake_minimum_required (VERSION 2.6)
project (Unique_Words_Counter)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")

find_package (Threads)
add_executable (main parser.cpp)
target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT})
