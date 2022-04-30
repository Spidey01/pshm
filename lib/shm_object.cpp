// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/shm_object.hpp>

#include <cerrno>
#include <cstring>

namespace pshm
{
    PSHM_DEFINE_ALLOCATION_TRACER(shm_object);

    shm_object::shm_object(const string_type& name, flags_type flags, size_type length, offset_type offset)
        : mName(name)
        , mFlags(flags)
        , mSize(length)
        , mOffset(offset)
    {
        PSHM_ALLOCATION_TRACER_ALLOCATED(mName);
    }

    shm_object::~shm_object()
    {
        PSHM_ALLOCATION_TRACER_DEALLOCATED(mName);
    }

    shm_object::string_type shm_object::name() const noexcept
    {
        return mName;
    }

    shm_object::flags_type shm_object::flags() const noexcept
    {
        return mFlags;
    }

    shm_object::size_type shm_object::size() const noexcept
    {
        return mSize;
    }

    shm_object::offset_type shm_object::offset() const noexcept
    {
        return mOffset;
    }

    shm_object::string_type shm_object::make_error(const string_type& msg) const
    {
        string_type s;

        s.append(mTag)
            .append(": ")
            .append(msg);

        return s;
    }

    shm_object::string_type shm_object::make_error(const string_type& msg, int error) const
    {
        return make_error(msg + ": " + strerror(error));
    }

    shm_object::string_type shm_object::make_error(const string_type& msg, const string_type& name, int error) const
    {
        return make_error(msg + ": " + name, error);
    }

    shm_object::string_type shm_object::strerror(int error) const noexcept
    {
        return std::strerror(error);
    }
} // namespace pshm
