// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

void run_test(const string& name)
{
    pshm::shm_ptr<int> ptr;
    if (ptr)
        throw TestFailure(name, "operator bool returned true when it should be false");

    if (ptr.get() != nullptr)
        throw TestFailure(name, "get() returned != nullptr");
}