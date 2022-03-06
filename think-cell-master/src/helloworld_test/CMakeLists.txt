add_executable(helloworld_test)

target_sources(helloworld_test
PRIVATE
  main.cpp
  sayhello.test.cpp
)

target_link_libraries(helloworld_test
PRIVATE
  $<TARGET_OBJECTS:helloworld>
  ::Catch2
)

add_test(
NAME
  "helloworld_test"
COMMAND
  helloworld_test
)