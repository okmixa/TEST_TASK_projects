add_library(helloworld STATIC)

target_include_directories(helloworld
INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(helloworld
PRIVATE
  sayHello.cpp
  sayHello.h
)
