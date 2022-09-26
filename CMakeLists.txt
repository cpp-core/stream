cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (stream VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET stream OR TARGET stream::stream)
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
  
  # Have cmake output the compilation commands and create a link to it
  # from the project root directory so the language server can find
  # it.
  #
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
set(CMAKE_BUILD_TYPE Release CACHE INTERNAL "Build type")

# Options for generating tests and documentation.
#
option(STREAM_TEST "Generate the tests." OFF)
option(STREAM_DOCS "Generate the docs." OFF)

# Output configuration
#
message("-- stream: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- stream: Included from: ${CMAKE_SOURCE_DIR}")
message("-- stream: STREAM_TEST ${STREAM_TEST}")
message("-- stream: STREAM_DOCS ${STREAM_DOCS}")
message("-- stream: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

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
include(GNUInstallDirs)
include(cmake/cpp_core_find_package.cmake)
cpp_core_find_package(tuple)

# Build the library
#
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
add_library(stream::stream ALIAS stream)

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(stream PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(stream PUBLIC tuple::tuple)

# Optionally configure the tests
#
if(STREAM_TEST)
  enable_testing()
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
