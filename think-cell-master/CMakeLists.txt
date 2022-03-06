cmake_minimum_required(VERSION 3.15)
project(think-cell)

### C++ ############################
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard required for build")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "is C++ stndard a requirement")
SET(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "enable/disable compiler specific extensions")

### External Libraries ##############
set(FindModuleDir ${PROJECT_SOURCE_DIR}/CMake CACHE STRING "path to directory containing the Find<Module>.cmake modules")
mark_as_advanced(FindModuleDir)
list(APPEND CMAKE_MODULE_PATH ${FindModuleDir})
find_package(Catch2 REQUIRED MODULE)

### Testing #########################
ENABLE_TESTING()
include(CTest)

### Main Body #######################
add_subdirectory(src)