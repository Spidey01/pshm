#ifndef PSHM_TESTING_TESTING__HPP
#define PSHM_TESTING_TESTING__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include "./TestFailure.hpp"

#include <pshm/stdcpp.hpp>

#if __unix__
#include <pshm/posix_shm_object.hpp>
#define SHM_OBJECT_TEST_USES_POSIX_SHM_OBJECT 1
#endif

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstring>
#include <limits>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::exception;
using std::logic_error;
using std::runtime_error;
using std::strerror;
using std::string;
using std::to_string;
using std::vector;

using shm_object_unique_ptr = std::unique_ptr<pshm::shm_object>;

template <class... Args>
shm_object_unique_ptr make_shm_object_unique_ptr(Args&&... args)
{
#if SHM_OBJECT_TEST_USES_POSIX_SHM_OBJECT
    using shm_object_impl = pshm::posix_shm_object;
#else
#error shm_object not implemented for this platform
#endif

    return std::make_unique<shm_object_impl>(std::forward<Args>(args)...);
}

/** Called from the main() in main.cpp.
 *
 * To create a new test case make a new file for the test case and link it and
 * main.cpp to provide a standard main() for tests.
 *
 * @param name the test program name, e.g., argv[0] stripped to the base file name.
 * @throws TestFailure on test failure.
 */
void run_test(const string& name);

/**
 * @brief Simple structure for testing.
 *
 * For stuffing in shared memory, effectively a
 */
class TestStruct
{
  public:
    static constexpr size_t Bounds = 16;
    int number;
    float real;
    bool true_or_false;
    char str[Bounds];
};

/* If you can't memcpy() it then it's not gonna be happy here. */
static_assert(std::is_trivial<TestStruct>::value, "Structure for shared memory needs to be a trivial type.");

/**
 * @brief Return the basename of the path.
 *
 * Similar to POSIX basename() and std::filesystem::path::filename() but without
 * the respective dependencies.
 *
 * @param path to basename.
 * @returns the basename.
 */
inline string basename_for_testing(const string& path)
{
    if (path.empty())
        return path;

#if defined(_WIN32)
    char sep = '\\';
#else
    char sep = '/';
#endif

    size_t i = path.rfind(sep);

    if (i == string::npos)
        return path;
    return path.substr(i + 1);
}

/**
 * @brief Decent approximation for floats.
 *
 * @param a first real number to compare.
 * @param b second real number to compare.
 * @param epsilon the approximation error.
 * @returns true if a and b are approximately equal.
 */
inline bool approximatelyEqual(float a, float b, float epsilon)
{
    return std::abs(a - b) <= (std::max(std::abs(a), std::abs(b)) * epsilon);
}

/**
 * @brief Assert that two test structures are equal.
 *
 * Unlike an operator== this will throw an exception denoting which field doesn't match.
 *
 * @param name passed to TestFailure on failure.
 * @param lhs left hand side.
 * @param rhs right hand side.
 * @throws TestFailure on the field fied that does not match.
 */
inline void assert_equal(const std::string& name, const TestStruct& lhs, const TestStruct& rhs)
{
    if (lhs.number != rhs.number)
        throw TestFailure(name, "number is " + to_string(lhs.number) + " but " + to_string(rhs.number) + " was expected");

    if (lhs.true_or_false != rhs.true_or_false)
        throw TestFailure(name, "true_or_false is " + to_string(lhs.true_or_false) + " but " + to_string(rhs.true_or_false) + " was expected");

    size_t lenptr = strlen(lhs.str);
    size_t lents = strlen(rhs.str);
    if (lenptr != lents)
        throw TestFailure(name, "strlen(lhs.string) is " + to_string(lenptr) + " but " + to_string(lents) + " was expected");

    int rc = strcmp(lhs.str, rhs.str);
    if (rc != 0)
        throw TestFailure(name, "strcmp(lhs.str, rhs.str) returned " + to_string(rc) + " but 0 was expected");

    /* A reasonable margin for values we used in our test cases. */
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    if (!approximatelyEqual(lhs.real, rhs.real, 0.1f))
        throw TestFailure(name, to_string(lhs.real) + " and " + to_string(rhs.real) + " differ by more than " + to_string(epsilon));
}

#endif // PSHM_TESTING_TESTING__HPP