// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

#include <iostream>
#include <memory>
#include <functional>

void run_test(const string& name)
{
    int n = 5;
    int* iptr = &n;
    pshm::shm_ptr<int> ptr;

    size_t iptr_hash = std::hash<int*>()(iptr);
    size_t ptr_hash = std::hash<pshm::shm_ptr<int>>()(ptr);

    cout
        << "iptr_hash: " << iptr_hash << endl
        << "ptr_hash: " << ptr_hash << endl
        << endl;
}
