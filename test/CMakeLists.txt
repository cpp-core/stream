cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

find_package(googletest REQUIRED)
find_package(Threads REQUIRED)

set(TESTS
  fp/either
  fp/fold
  fp/option
  fp/try
  )

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configure_tests.cmake)
set(LIBS "cxx_core_mp::cxx_core_mp;googletest::cxx-googletest;Threads::Threads")
configure_tests("core" "${LIBS}" ${TESTS})
