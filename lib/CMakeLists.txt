cmake_minimum_required(VERSION 3.18..3.23 FATAL_ERROR)

add_library(pshm
    shm_object.cpp)

# uname -s, or "Windows"
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # TODO
    # target_sources(pshm PRIVATE shm_object_win32.cpp)
elseif (UNIX)
    target_sources(pshm PRIVATE posix_shm_object.cpp)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(pshm rt Threads::Threads)
endif()

target_include_directories(pshm PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS pshm
    EXPORT pshmTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/pshm"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

include(pshm_config)
