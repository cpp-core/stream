cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

function(install_config TARGET)
  include(GNUInstallDirs)
  
  if(${ARGC} GREATER_EQUAL 2)
    set(NAME ${ARGV1})
  else()
    set(NAME ${TARGET})
  endif()
  set(FLAVOR ${TARGET}-component)
  
  install(TARGETS ${TARGET}
    EXPORT ${NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${FLAVOR})

  set(CONFIG_VERSION_NAME "${NAME}ConfigVersion.cmake")
  set(CONFIG_NAME "${NAME}Config.cmake")
  set(CONFIG_FILE_IN "${PROJECT_SOURCE_DIR}/cmake/${CONFIG_NAME}.in")
  set(CONFIG_FILE "${PROJECT_BINARY_DIR}/${CONFIG_NAME}")
  
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(${CONFIG_VERSION_NAME}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)
  
  configure_package_config_file(${CONFIG_FILE_IN} ${CONFIG_FILE}
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${NAME}/cmake)
  
  install(EXPORT ${NAME}_Targets
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${NAME}/cmake
    NAMESPACE ${NAME}::
    FILE ${NAME}Targets.cmake
    COMPONENT ${FLAVOR})

  install(FILES
    "${CONFIG_FILE}"
    "${PROJECT_BINARY_DIR}/${CONFIG_VERSION_NAME}"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${NAME}/cmake
    COMPONENT ${FLAVOR})

  add_custom_target(${TARGET}-install
    DEPENDS ${TARGET}
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT=${FLAVOR}
    -P ${CMAKE_BINARY_DIR}/cmake_install.cmake)
endfunction()
