cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (stream VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET stream OR TARGET stream::stream)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)

  # Options for generating tests and documentation.
  #
  option(STREAM_TEST "Generate the tests." ON)
  option(STREAM_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()

else()
  option(STREAM_TEST "Generate the tests." OFF)
  option(STREAM_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- stream: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- stream: Included from: ${CMAKE_SOURCE_DIR}")
message("-- stream: Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("-- stream: test ${STREAM_TEST}")
message("-- stream: docs ${STREAM_DOCS}")

# Setup the compilation environment before bringing in the dependencies.
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

# Add our dependencies
#
add_tuple()

# Build the library
#
set(SOURCES
  stream/detail/random
  stream/io/read_lines
  stream/sampler/char
  stream/sampler/string
  )

set(FILES)
foreach(NAME ${SOURCES})
  list(APPEND FILES "src/coro/${NAME}.cpp")
endforeach()

add_library(stream ${FILES})
add_library(stream::stream ALIAS stream)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(stream PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(stream PUBLIC tuple::tuple)

# Optionally configure the tests
#
if(STREAM_TEST)
  add_gtest()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(STREAM_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
install_config(stream)
