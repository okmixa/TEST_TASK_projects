cmake_minimum_required(VERSION 3.1)

project(udp_client LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set(CMAKE_AUTOMOC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_USE_QTOPENGL TRUE)
set(CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")
set_target_properties(
PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS  true
)

INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )

find_package(OpenGL REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core OpenGL Gui Widgets OpenGL Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core OpenGL Gui Widgets OpenGL Network REQUIRED)

set(SOURCES main.cpp udp_client.cpp opengl_widget.cpp)
set(HEADERS udp_client.h opengl_widget.h)

add_executable(udp_client
  ${SOURCES} ${HEADERS}
)

target_link_libraries(udp_client Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets ${OPENGL_LIBRARIES} Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::Network)

