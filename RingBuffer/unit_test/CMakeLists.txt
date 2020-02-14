CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

project("test")

ENABLE_LANGUAGE(CXX)
SET(CMAKE_BUILD_TYPE Release)

IF(UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -ldl")
ENDIF(UNIX)

IF(MSVC)
   MESSAGE("-- cmake for Visual Studio ")
ENDIF(MSVC)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test test_circle_buff.cpp)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})
target_link_libraries(test libcircle_buffer ${CMAKE_DL_LIBS})
