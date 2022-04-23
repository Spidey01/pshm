#ifndef PSHM_TESTING_TESTFAILURE__hpp
#define PSHM_TESTING_TESTFAILURE__hpp
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <stdexcept>
#include <string>

/** Exception to throw if test fails. */
class TestFailure : public std::exception
{
  private:
    std::string msg;

  public:
    TestFailure(const std::string& name, const std::string& message, std::exception& related)
        : msg(name + ": " + message + ": " + std::string(related.what()))
    {
    }

    TestFailure(const std::string& name, const std::string& message)
        : msg(name + ": " + message)
    {
    }

    TestFailure(const std::string& msg)
        : msg(msg)
    {
    }

    // N.B. C++11: noexcept; C++03: throw().
    const char* what() const noexcept
    {
        return msg.c_str();
    }
};

#endif // PSHM_TESTING_TESTFAILURE__hpp