
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
ENABLE_LANGUAGE(CXX)
SET(CMAKE_BUILD_TYPE Release)

IF(UNIX)
   MESSAGE("-- cmake for *NIX ")
   SET(CMAKE_CXX_FLAGS "-Wall -rdynamic -Wunused -std=c++11 ${CMAKE_CXX_FLAGS_DEBUG} -ldl -pthread -D_GLIBCXX_USE_NANOSLEEP")
ENDIF(UNIX)

IF(MSVC)
   MESSAGE("-- cmake for Visual Studio ")
ENDIF(MSVC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(libcircle_buffer SHARED circle_buffer.cpp)
