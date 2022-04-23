// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

#include <pshm/shm_object.hpp>

void run_test(const string& name)
{
    using pshm::flags;
    pshm::shm_object::size_type length = sizeof(int);
    pshm::shm_object::offset_type offset = 0;

    cout << "Trying " << name << " with flags::EXCL without flags::CREAT so it fails" << endl;
    try {
        auto bad_excl = make_shm_object_unique_ptr(name, flags::EXCL, length, offset);
        throw TestFailure(name, "flags::EXCL should fail without flags::CREAT");
    } catch (runtime_error& ex) {
        cout << "Good, got expected runtime_error: " << ex.what() << endl;
    }

    pshm::flags_t excl = flags::RDWR | flags::CREAT | flags::EXCL;
    cout << "Creating " << name << " with pshm::flags::EXCL to watch it succeed" << endl;
    shm_object_unique_ptr good_excl;
    try {
        /* If proper clean up in dtor fails: this will fail on subsequent tests. */
        good_excl = make_shm_object_unique_ptr(name, excl, length, offset);
    } catch (runtime_error& ex) {
        throw TestFailure(name, "RDWR, CREATE, EXCL failed", ex);
    }

    cout << "Trying " << name << " again with pshm::flags::EXCL to watch it fail" << endl;
    try {
        auto fail_excl = make_shm_object_unique_ptr(name, excl, length, offset);
        throw TestFailure(name, "RDWR, CREATE, EXCL succeeded when the segment already exists");
    } catch (runtime_error& ex) {
        cout << "Good, got expected runtime_error: " << ex.what() << endl;
    }
}
