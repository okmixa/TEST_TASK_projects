add_executable(interval_map_test)

target_sources(interval_map_test
PRIVATE
  main.cpp
  interval_map.test.cpp
  key.test.cpp
  value.test.cpp
)

target_link_libraries(interval_map_test
PRIVATE
  interval_map
  ::Catch2
)

add_test(
NAME
  "interval_map_test"
COMMAND
  interval_map_test
)