// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

#include <pshm/shm_object.hpp>

void run_test(const string& name)
{
    using pshm::flags;
    pshm::shm_object::size_type length = sizeof(int);
    pshm::shm_object::offset_type offset = 0;

    cout << "Trying " << name << " with RDONLY to watch it fail." << endl;
    try {
        shm_object_unique_ptr ptr = make_shm_object_unique_ptr(name, flags::RDONLY, 4, 0);
        throw TestFailure(name, "RDONLY without CREAT is supposed to fail instead of creating the segment");
    } catch (runtime_error& ex) {
        cout << "Good, got expected exception: " << ex.what() << endl;
    }

    cout << "Trying " << name << " with RDONLY | CREAT | EXCL to watch it succeed." << endl;
    try {
        shm_object_unique_ptr ptr = make_shm_object_unique_ptr(name, flags::RDONLY | flags::CREAT | flags::EXCL, 4, 0);
    } catch (runtime_error& ex) {
        throw TestFailure(name, "Failed on RDONLY|CREAT|EXCL", ex);
    }
}
