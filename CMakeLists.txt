cmake_minimum_required(VERSION 3.18..3.23 FATAL_ERROR)

project(pshm
    VERSION 0.0.0
    LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build libs as shared." ON)
option(BUILD_TESTING "Enable CTest support." ON)
option(BUILD_DOCS "Enable Doxygen support." ON)
option(PSHM_TRACING "Enable tracing to std::cout" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

include(GNUInstallDirs)

install(FILES
    README.md
    CHANGELOG.md
    LICENSE.txt
    NOTICE
    INSTALL.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

if (BUILD_DOCS)
    include(pshm_doxygen)
endif (BUILD_DOCS)

add_subdirectory(include)
add_subdirectory(lib)
if (BUILD_TESTING)
    include(CTest)
    add_subdirectory(testing)
endif (BUILD_TESTING)
