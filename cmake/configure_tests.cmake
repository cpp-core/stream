cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

function(configure_tests SUBDIR LIBRARIES)
  if (NOT TARGET check)
    add_custom_target(check ${CMAKE_CTEST_COMMAND})
    add_custom_target(check_detail ${CMAKE_CTEST_COMMAND} -V)
  endif()
  
  foreach(NAME ${ARGN})
    get_filename_component(DIRNAME ${NAME} DIRECTORY)
    get_filename_component(BASENAME ${NAME} NAME_WE)
    string(REPLACE / _ CATNAME ${NAME})

    set(TEST_NAME test_${CATNAME})
    set(TEST_FILE ${PROJECT_SOURCE_DIR}/test/src/${SUBDIR}/${DIRNAME}/test_${CATNAME}.cpp)
    if(NOT EXISTS ${TEST_FILE})
      message(FATAL "Test: ${TEST_NAME}, Failed to find test file: ${TEST_FILE}")
    endif()

    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${TEST_FILE})
    add_test(${TEST_NAME} bin/${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE ${LIBRARIES})
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

    add_dependencies(check ${TEST_NAME})
    add_dependencies(check_detail ${TEST_NAME})
  endforeach(NAME)
endfunction()

