# vim: set filetype=cmake tabstop=4 shiftwidth=4 expandtab :

include(CMakePackageConfigHelpers)

set(project_pkgconfig_file ${PROJECT_NAME}Config.cmake)
set(project_pkgversion_file ${PROJECT_NAME}Config.cmake)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${project_pkgconfig_file}
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(${PROJECT_BINARY_DIR}/${project_pkgversion_file}
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${PROJECT_BINARY_DIR}/${project_pkgconfig_file}
    ${PROJECT_BINARY_DIR}/${project_pkgversion_file}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME})