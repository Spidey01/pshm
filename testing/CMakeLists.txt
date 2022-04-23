cmake_minimum_required(VERSION 3.18..3.23 FATAL_ERROR)

# N.B.: WIN32: can't find pshm.lib when BUILD_SHARED_LIBS.

# add_executable(compiler_features compiler_features.cpp)
# add_test(test_compiler_features compiler_features)


add_executable(shm_object_create_excl shm_object_create_excl.cpp main.cpp)
target_link_libraries(shm_object_create_excl pshm)
add_test(test_shm_object_create_excl shm_object_create_excl)

add_executable(shm_object_create_ro shm_object_create_ro.cpp main.cpp)
target_link_libraries(shm_object_create_ro pshm)
add_test(test_shm_object_create_ro shm_object_create_ro)

add_executable(shm_object_create_rw shm_object_create_rw.cpp main.cpp)
target_link_libraries(shm_object_create_rw pshm)
add_test(test_shm_object_create_rw shm_object_create_rw)

add_executable(shm_object_fields shm_object_fields.cpp main.cpp)
target_link_libraries(shm_object_fields pshm)
add_test(test_shm_object_fields shm_object_fields)

add_executable(shm_ptr_default_constructor shm_ptr_default_constructor.cpp main.cpp)
target_link_libraries(shm_ptr_default_constructor pshm)
add_test(test_shm_ptr_default_constructor shm_ptr_default_constructor)

add_executable(shm_ptr_nullptr_constructor shm_ptr_nullptr_constructor.cpp main.cpp)
target_link_libraries(shm_ptr_nullptr_constructor pshm)
add_test(test_shm_ptr_nullptr_constructor shm_ptr_nullptr_constructor)

add_executable(shm_ptr_move_constructor shm_ptr_move_constructor.cpp main.cpp)
target_link_libraries(shm_ptr_move_constructor pshm)
add_test(test_shm_ptr_move_constructor shm_ptr_move_constructor)

add_executable(shm_ptr_move_assignment shm_ptr_move_assignment.cpp main.cpp)
target_link_libraries(shm_ptr_move_assignment pshm)
add_test(test_shm_ptr_move_assignment shm_ptr_move_assignment)

add_executable(shm_ptr_nullptr_assignment shm_ptr_nullptr_assignment.cpp main.cpp)
target_link_libraries(shm_ptr_nullptr_assignment pshm)
add_test(test_shm_ptr_nullptr_assignment shm_ptr_nullptr_assignment)

add_executable(shm_ptr_hash_specialization shm_ptr_hash_specialization.cpp main.cpp)
target_link_libraries(shm_ptr_hash_specialization pshm)
add_test(test_shm_ptr_hash_specialization shm_ptr_hash_specialization)

add_executable(shm_ptr_swap shm_ptr_swap.cpp main.cpp)
target_link_libraries(shm_ptr_swap pshm)
add_test(test_shm_ptr_swap shm_ptr_swap)

add_executable(shm_ptr_get shm_ptr_get.cpp main.cpp)
target_link_libraries(shm_ptr_get pshm)
add_test(test_shm_ptr_get shm_ptr_get)

if (UNIX)
    add_executable(shm_ptr_fork shm_ptr_fork.cpp main.cpp)
    target_link_libraries(shm_ptr_fork pshm)
    add_test(test_shm_ptr_fork shm_ptr_fork)
endif (UNIX)
