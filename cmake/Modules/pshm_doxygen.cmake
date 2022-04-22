
if("${CMAKE_VERSION}" VERSION_LESS "3.12") 
    message("cmake >= 3.9 required for doxygen_add_docs().")
    message("cmake >= 3.12 required for ALL argument to doxygen_add_docs().")
else()
    find_package(Doxygen)
endif()

if (DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_BRIEF "Portable C++ shared memory library.")
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_STRIP_FROM_INC_PATH ${PROJECT_SOURCE_DIR}/include)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${project_readme}")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)

    message("project_readme: ${project_readme}")
    doxygen_add_docs(docs
        ${PROJECT_SOURCE_DIR}/include/
        ${PROJECT_SOURCE_DIR}/INSTALL.md
        ${PROJECT_SOURCE_DIR}/CHANGELOG.md
        ALL
        COMMENT "Generate documentation with Doxygen")

    install(DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
            DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif (DOXYGEN_FOUND)
