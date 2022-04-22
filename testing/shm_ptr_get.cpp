// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.
#include "./testing.hpp"

#include <pshm/shm_ptr.hpp>

void run_test(const string& name)
{
    pshm::shm_ptr<TestStruct> ptr("shm_ptr_get");
    cout << "ptr: " << ptr << endl;

    if (!ptr)
        throw TestFailure(name, "ptr should be valid");

    auto star1 = *ptr;
    auto star2 = *(ptr.get());
    assert_equal(name, star1, star2);

    if (ptr->number != ptr.get()->number)
        throw TestFailure(name, "ptr-> should be like ptr.get()");

    TestStruct ts;

    cout << "Setup TestStruct ts" << endl;
    ts.number = 12345;
    ts.real = 2.5f;
    ts.true_or_false = true;
    static const char* hello = "Hello";
    memset(ts.str, '\0', TestStruct::Bounds);
    strncpy(ts.str, hello, std::min(strlen(hello), TestStruct::Bounds));

    cout << "memcpy TestStruct ts to ptr" << endl;
    memcpy(ptr.get(), &ts, sizeof(TestStruct));

    assert_equal(name, *ptr, ts);

    class SimplePants
    {
      public:
        int n;
        char buffer[200];
    };

    pshm::shm_ptr<SimplePants> okay;

    class FancyPants
    {
      public:
        FancyPants(int n, std::string& s)
            : mN(n)
            , mS(s)
        {
        }

      private:
        int mN;
        std::string mS;
    };

    // pshm::shm_ptr<FancyPants> error;
}
