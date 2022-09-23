cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(TESTS
  stream/base
  stream/generator
  stream/io
  stream/samplers
  stream/types
  stream/util
  )

set(LIBRARIES
  stream
  tuple::tuple
  fmt::fmt
  GTest::gtest
  Threads::Threads)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
configure_tests("coro" "${LIBRARIES}" ${TESTS})
