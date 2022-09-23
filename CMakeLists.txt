cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (stream VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET stream)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(STREAM_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  
  # Have cmake output the compilation commands
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Options for generating tests and documentation.
#
option(STREAM_TEST "Generate the tests." OFF)
option(STREAM_DOCS "Generate the docs." OFF)

# Add our dependencies
#
include(GNUInstallDirs)
find_package(tuple REQUIRED)
find_package(fmt REQUIRED)

# Output configuration
#
message("-- stream: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- stream: Included from: ${CMAKE_SOURCE_DIR}")
message("-- stream: STREAM_TEST ${STREAM_TEST}")
message("-- stream: STREAM_DOCS ${STREAM_DOCS}")
message("-- stream: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

# Compilation
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

set(SOURCES
  stream/detail/random
  stream/io/read_lines
  stream/sampler/char
  stream/sampler/string
  )

foreach(NAME ${SOURCES})
  list(APPEND FILES "src/coro/${NAME}.cpp")
endforeach()

add_library(stream ${FILES})

target_link_libraries(stream PUBLIC tuple::tuple)

target_include_directories(stream
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Optionally configure the tests
#
if(STREAM_TEST)
  enable_testing()
  if (NOT TARGET check)
    add_custom_target(check ${CMAKE_CTEST_COMMAND})
    add_custom_target(check_detail ${CMAKE_CTEST_COMMAND} -V)
  endif()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(STREAM_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_config.cmake)
install_config(stream)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/coro/stream DESTINATION include/coro)
