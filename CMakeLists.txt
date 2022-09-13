cmake_minimum_required (VERSION 3.22 FATAL_ERROR)
project (cxx_core_fp VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET cxx_core_fp)
  return()
endif()

# Configuration variables.
#
# Build the tests and set directories if this is the master project
#
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CORE_FP_TEST ON CACHE BOOL "Generate the tests.")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_DOCS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  
  # Have cmake output the compilation commands
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Options for generating tests and documentation.
#
option(CORE_FP_TEST "Generate the tests." OFF)
option(CORE_FP_DOCS "Generate the docs." OFF)

# Add our dependencies
#
include(GNUInstallDirs)
find_package(cxx_core_mp)

# Output configuration
#
message("-- cxx-core-fp: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- cxx-core-fp: Included from: ${CMAKE_SOURCE_DIR}")
message("-- cxx-core-fp: CORE_TUPLE_TEST ${CORE_FP_TEST}")
message("-- cxx-core-fp: CORE_TUPLE_DOCS ${CORE_FP_DOCS}")
message("-- cxx-core-fp: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

# Compilation
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

add_library(cxx_core_fp INTERFACE)
target_include_directories(cxx_core_fp
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Optionally configure the tests
#
if(CORE_FP_TEST)
  enable_testing()
  if (NOT TARGET check)
    add_custom_target(check ${CMAKE_CTEST_COMMAND})
    add_custom_target(check_detail ${CMAKE_CTEST_COMMAND} -V)
  endif()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
# if(CORE_FP_DOCS)
#   add_subdirectory(docs)
# endif()

# Installation
#
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install_config.cmake)
install_config(cxx_core_fp)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/core/fp DESTINATION include/core/)
