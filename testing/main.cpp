// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./testing.hpp"

// For C++14.
constexpr size_t TestStruct::Bounds;

/*
 * Standard entry point linked into each test program.
 *
 * Calls the run_test() to be provided by the test program's own source file
 * with some standard I/O and exception wrangling.
 */
int main(int argc, char* argv[])
{
    const string name = basename_for_testing(argv[0]);
    int rc = 1;

    try {
        cout << "Running test " << name << endl;
        run_test(name);
        rc = 0;
    } catch (runtime_error& re) {
        cout << name << ": runtime_error: " << re.what() << endl;
    } catch (exception& ex) {
        cout << name << ": exception: " << ex.what() << endl;
    }

    return rc;
}
