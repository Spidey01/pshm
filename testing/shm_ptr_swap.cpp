// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

void run_test(const string& name)
{
    constexpr int value = 12345;

    pshm::shm_ptr<int> p1;
    pshm::shm_ptr<int> p2(name);

    if (!std::is_swappable_v<pshm::shm_ptr<int>>)
        throw TestFailure(name, "!is_swappable_v");

    *p2 = value;

    cout << "p1: " << p1 << endl;
    cout << "*p2: " << *p2 << endl;

    cout << "std::swap(p1, p2)" << endl;
    std::swap(p1, p2);

    cout << "*p1: " << *p1 << endl;
    cout << "p2: " << p2 << endl;

    if (*p1 != value)
        throw TestFailure(name, "*p1 is " + to_string(*p1) + " but " + to_string(value) + " was expected ");
    if (p2)
        throw TestFailure(name, "p2 not nullptr after swap");
}