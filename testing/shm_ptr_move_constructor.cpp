// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

void run_test(const string& name)
{
    pshm::shm_ptr<int> p1("shm_ptr_move_constructor");

    pshm::shm_ptr<int> p2(std::move(p1));

    if (p1)
        throw TestFailure(name, "p1: operator bool returned true when it should be false");
    if (p1.get() != nullptr)
        throw TestFailure(name, "p1: get() returned != nullptr");

    if (!p2)
        throw TestFailure(name, "p2: operator bool returned false when it should be true");
    if (p2.get() == nullptr)
        throw TestFailure(name, "p2: get() returned == nullptr");
}