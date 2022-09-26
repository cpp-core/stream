cmake_minimum_required (VERSION 3.22 FATAL_ERROR)

function(install_config TARGET)
  cmake_parse_arguments(
    IC
    "" # Flags
    "" # Single Value Options
    "DEPENDS" # Multi-Value Options
    ${ARGN}
    )
  set(target_depends ${IC_DEPENDS})
  
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)
  
  set(flavor ${TARGET}-component)

  # This is where the cmake config* files will be installed.
  set(config_install_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET})

  # The config version file.
  set(config_version_name ${TARGET}ConfigVersion.cmake)
  set(config_version_build ${PROJECT_BINARY_DIR}/${config_version_name})

  # The config file.
  set(config_name ${TARGET}Config.cmake)
  set(config_install ${config_install_dir}/${config_name})

  # Building the config file.
  set(config_in "${PROJECT_SOURCE_DIR}/cmake/${config_name}.in")
  set(config_build "${PROJECT_BINARY_DIR}/${config_name}")

  # The config target.
  set(config_target_name "${TARGET}Targets")
  set(config_target_build ${PROJECT_BINARY_DIR}/${config_target_name}.cmake)
  set(config_target_install ${cmake_install_dir}/${config_target_name}.cmake)
  
  write_basic_package_version_file(
    ${config_version_name}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    )

  configure_package_config_file(
    ${config_in}
    ${config_build}
    INSTALL_DESTINATION ${config_install_dir}
    )

  install(
    TARGETS ${TARGET} ${target_depends}
    EXPORT ${config_target_name}
    ARCHIVE
    LIBRARY
    RUNTIME
    PUBLIC_HEADER
    FILE_SET HEADERS
    COMPONENT ${flavor}
    )
  
  export(
    TARGETS ${TARGET}
    NAMESPACE ${TARGET}::
    FILE ${config_target_build}
    )

  foreach(DT ${target_depends})
    export(
      TARGETS ${DT}
      APPEND FILE ${config_target_build}
      )
  endforeach()

  install(
    FILES ${config_build} ${config_version_build}
    DESTINATION ${config_install_dir}
    COMPONENT ${flavor}
    )

  install(
    EXPORT ${config_target_name}
    DESTINATION ${config_install_dir}
    NAMESPACE ${TARGET}::
    COMPONENT ${flavor}
    )

  add_custom_target(
    ${TARGET}-install
    DEPENDS ${TARGET}
    COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_COMPONENT=${flavor}
    -P ${CMAKE_BINARY_DIR}/cmake_install.cmake)
  
endfunction()
