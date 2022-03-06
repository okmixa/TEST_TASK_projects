add_library(interval_map INTERFACE)

target_include_directories(interval_map
INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_sources(interval_map
INTERFACE
  interval_map.h
  key.h
  value.h
)
