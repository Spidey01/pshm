// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

#include <pshm/shm_object.hpp>

void run_test(const string& name)
{
    using pshm::flags;
    pshm::shm_object::size_type length = sizeof(int);
    pshm::shm_object::offset_type offset = 20;
    pshm::flags_t oflags = flags::RDWR | flags::CREAT | flags::EXCL;

    cout << "Creating " << name << endl;
    shm_object_unique_ptr ptr;
    try {
        ptr = make_shm_object_unique_ptr(name, oflags, length, offset);
    } catch (runtime_error& ex) {
        throw TestFailure(name, "Failed on RDWR|CREAT|EXCL", ex);
    }

    if (!ptr)
        throw TestFailure(name, "Where's my pointer?");
    
    if (ptr->name().empty())
        throw TestFailure(name, "shm_object::name() can't be empty and still valid.");

    if ((ptr->flags() & flags::RDWR) == 0)
        throw TestFailure(name, "shm_object::flags() missing flags::RDWR");
    if ((ptr->flags() & flags::CREAT) == 0)
        throw TestFailure(name, "shm_object::flags() missing flags::CREAT");
    if ((ptr->flags() & flags::EXCL) == 0)
        throw TestFailure(name, "shm_object::flags() missing flags::EXCL");
    if ((ptr->flags() & oflags) == 0)
        throw TestFailure(name, "shm_object::flags() missing some flags or test out of date");

    /* Currently this is the size requested, not the physically allocated. So it
     * should always match.
     */
    if (ptr->size() != length)
        throw TestFailure(name, "shm_object::size() doesn't match requested length");
    if (ptr->offset() != offset)
        throw TestFailure(name, "shm_object::offset() doesn't match requested offset");

    if (ptr->get() == nullptr)
        throw TestFailure(name, "shm_object::get() returned nullptr but should be real.");
}
