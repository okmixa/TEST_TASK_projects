find_path(Catch2_INCLUDE_DIR
NAMES
  catch.hpp
PATHS
  ${CMAKE_SOURCE_DIR}/external/Catch2/
)

set(Catch2_VERSION "2.13")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Catch2
FOUND_VAR Catch2_FOUND
REQUIRED_VARS
  Catch2_INCLUDE_DIR
)

if (Catch2_FOUND AND NOT TARGET ::Catch2)
  add_library(::Catch2 INTERFACE IMPORTED)
  set_target_properties(::Catch2 PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES
    ${Catch2_INCLUDE_DIR}
  )
endif()

mark_as_advanced(
    Catch2_INCLUDE_DIR
)