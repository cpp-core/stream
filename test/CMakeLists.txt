cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

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
  GTest::gtest
  Threads::Threads)

configure_tests("coro" "${LIBRARIES}" ${TESTS})
