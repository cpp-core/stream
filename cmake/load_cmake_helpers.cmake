
if(CPP_CORE_DEV AND NOT CPP_CORE_DIR)
  set(CPP_CORE_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

if(CPP_CORE_DIR AND EXISTS ${CPP_CORE_DIR}/cmake)
  set(CPP_CORE_CMAKE_DIR ${CPP_CORE_DIR}/cmake)
else()
  include(FetchContent)
  FetchContent_Declare(
    cmake
    GIT_REPOSITORY https://github.com/cpp-core/cmake
    GIT_TAG main
    )
  FetchContent_MakeAvailable(cmake)
  set(CPP_CORE_CMAKE_DIR ${CMAKE_BINARY_DIR}/_deps/cmake-src)
endif()

include(${CPP_CORE_CMAKE_DIR}/utils/all.cmake)
include(${CPP_CORE_CMAKE_DIR}/recipes/all.cmake)
