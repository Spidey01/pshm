// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/flags.hpp>

namespace pshm
{
    /* C++14 requires these to exist like this. C++17 is happy enough with the header. */

    constexpr flags_t flags::RDONLY;
    constexpr flags_t flags::RDWR;
    constexpr flags_t flags::CREAT;
    constexpr flags_t flags::EXCL;
    constexpr flags_t flags::TRUNC;

} // namespace pshm
