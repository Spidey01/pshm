
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

    cout << "Creating RDWR " << name << endl;
    shm_object_type o1(name, oflags, length, offset);
    if (o1.get() == nullptr)
        throw TestFailure(name, "o1 is nullptr");

    /* No default ctor, so must be a real one at the move point. */
    cout << "Creating RO " << name << endl;
    shm_object_type o2(name, flags::RDONLY, length, offset);
    if (o2.get() == nullptr)
        throw TestFailure(name, "o2 is nullptr");

    /*
     * While it's not necessary for the pointer to be the same between to
     * instances, basically every platform with virtual memory and anything
     * _like_ mmap() will simply reuse the address thanks to reference counting.
     */
    if (o1.get() == o2.get())
        cout << "o1.get() == o2.get()" << endl;

    void* p = o1.get();

    cout << "Move assigning the RDWR to the RO" << endl;
    o2 = std::move(o1);
    if (o2.get() == nullptr)
        TestFailure(name, "o2.get() is nullptr after the move construction");
    if (p != o2.get()) {
        TestFailure(name, "after move assignment o2.get() is not the same pointer as o1.get() was before transfer.");
    }
    if (o1.get() != nullptr)
        TestFailure(name, "o1.get() is not nullptr after the move");
}
