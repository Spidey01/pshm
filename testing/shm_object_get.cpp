// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

#include <pshm/shm_object.hpp>

void run_test(const string& name)
{
    using pshm::flags;
    constexpr size_t Bounds_Units = 5;
    constexpr size_t Bounds_Bytes = sizeof(uint32_t) * Bounds_Units;
    pshm::shm_object::size_type length = Bounds_Bytes + 1;
    pshm::shm_object::offset_type offset = 0;
    pshm::flags_t oflags = flags::RDWR | flags::CREAT | flags::EXCL;

    cout << "Creating " << name << endl;
    shm_object_unique_ptr ptr = make_shm_object_unique_ptr(name, oflags, length, offset);

    if (ptr->get() == nullptr)
        throw TestFailure(name, "shm_object::get() returned nullptr but should be real.");

    cout << "Writing zeros to " << name << endl;
    memset(ptr->get(), 0, ptr->size());

    uint32_t* ap = (uint32_t*)ptr->get();
    for (size_t i = 0; i < Bounds_Bytes / sizeof(uint32_t); ++i) {
        cout << "ap[" << i << "]: " << ap[i] << endl;
        if (*ap != 0)
            throw TestFailure(name, to_string(i) + "'th uint32_t is not zero");
    }

    cout << "Writing INT_MAX to first int in " << name << endl;

    uint32_t expected[Bounds_Units] = {
        0x01010101, /* 16843009 aka 0b00000001000000010000000100000001 */
        UINT32_MAX,
        0x80808080, /* 2155905152 aka 0b10000000100000001000000010000000 */
        0xDEADBEEF, /* 3735928559 aka 0b11011110101011011011111011101111 */
        0xCAFEBABE, /* 3405691582 aka 0b11001010111111101011101010111110 */
    };

    for (size_t i = 0; i < Bounds_Units; ++i) {
        cout << "Setting " << i << "'th uint32 to " << expected[i] << endl;
        memcpy((uint32_t*)ptr->get() + i, &expected[i], sizeof(uint32_t));
    }

    for (size_t i = 0; i < Bounds_Units; ++i) {
        cout << "Checking that " << i << "'th uint32 is now " << expected[i] << endl;
        if (ap[i] != expected[i])
            throw TestFailure(name, "ap[" + to_string(i) + "] is " + to_string(ap[i]) + " but " + to_string(expected[i]) + " was expected");
    }
}
