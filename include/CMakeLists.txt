cmake_minimum_required(VERSION 3.18..3.23 FATAL_ERROR)

# Create config.hpp.
# -- goes in build tree, not source tree!
# -- and it wants to know these are set.
if (PROJECT_VERSION_PATCH)
    set(PSHM_VERSION_HAS_PATCH)
endif(PROJECT_VERSION_PATCH)
if (PROJECT_VERSION_MINOR)
    set(PSHM_VERSION_HAS_MINOR)
endif(PROJECT_VERSION_MINOR)
if (PROJECT_VERSION_MAJOR)
    set(PSHM_VERSION_HAS_MAJOR)
endif(PROJECT_VERSION_MAJOR)
set(CONFIG_HPP_ROOT ${PROJECT_BINARY_DIR}/include)
set(CONFIG_HPP ${CONFIG_HPP_ROOT}/pshm/config.hpp)
configure_file(pshm/config.hpp.in ${CONFIG_HPP} @ONLY)

install(FILES
    ${CONFIG_HPP}
    pshm/allocation_tracer.hpp
    pshm/flags.hpp
    pshm/posix_shm_object.hpp
    pshm/shm_object.hpp
    pshm/shm_object_native.hpp
    pshm/shm_ptr.hpp
    pshm/stdcpp.hpp
    pshm/tracing.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pshm)
