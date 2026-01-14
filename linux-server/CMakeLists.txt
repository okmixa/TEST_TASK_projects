cmake_minimum_required(VERSION 3.16)
project(linux_client_server VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Network Widgets Test REQUIRED QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Network Widgets Test REQUIRED QUIET)
    set(QT_VERSION 5)
else()
    set(QT_VERSION 6)
endif()

add_subdirectory(server)
add_subdirectory(client)

option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
