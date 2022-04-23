// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

#include <pshm/shm_object.hpp>
#include <pshm/shm_ptr.hpp>

void run_test(const string& name)
{
    using pshm::flags;
    pshm::shm_object::size_type length = sizeof(int);
    pshm::shm_object::offset_type offset = 0;
    pshm::flags_t oflags = flags::RDWR | flags::CREAT;

    using shm_object_type = pshm::shm_ptr<int>::shm_object_type;

    cout << "Creating " << name << endl;
    shm_object_type o1(name, oflags, length, offset);
    if (o1.get() == nullptr)
        throw TestFailure(name, "o1 is nullptr");

    cout << "Move constructing" << endl;
    shm_object_type o2(std::move(o1));
    if (o2.get() == nullptr)
        TestFailure(name, "o2.get() is nullptr after the move construction");
    if (o1.get() != nullptr)
        TestFailure(name, "o1.get() is not nullptr after the move");
}
