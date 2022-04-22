// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/posix_shm_object.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using std::invalid_argument;
using std::runtime_error;
using std::string;

namespace
{
    mode_t default_mode = 0600;

    /**
     * @brief Convert from portable flags to posix.
     *
     * @param i the flags to convert from pshm::flags.
     * @returns comparable POSIX fcntl flags.
     */
    int to_fcntl(pshm::posix_shm_object::flags_type i);

    string make_name(const string& name);

    int to_fcntl(pshm::posix_shm_object::flags_type i)
    {
        int o = 0;

        if (i & pshm::flags::CREAT)
            o |= O_CREAT;
        if (i & pshm::flags::RDONLY)
            o |= O_RDONLY;
        if (i & pshm::flags::RDWR)
            o |= O_RDWR;
        if (i & pshm::flags::EXCL)
            o |= O_EXCL;
        if (i & pshm::flags::TRUNC)
            o |= O_TRUNC;

        return o;
    }

    string make_name(const string& name)
    {
        if (name.empty())
            throw invalid_argument("shared memory object requires a name");

        if (name.front() == '/')
            return name;

        return "/" + name;
    }
} // namespace

namespace pshm
{
    PSHM_DEFINE_ALLOCATION_TRACER(posix_shm_object);

    posix_shm_object::posix_shm_object(const string_type& name, flags_type flags, size_type length, offset_type offset)
        : shm_object(make_name(name), flags, length, offset)
        , mPointer(nullptr)
        , mFile(-1)
    {
        string_type n = this->name();
        int f = to_fcntl(this->flags());
        mFile = shm_open(n.c_str(), f, default_mode);

        if (mFile == -1)
            throw runtime_error(make_error("shm_open() failed", n, errno));

        if (f & O_RDWR) {
            if (ftruncate(mFile, size()) != 0) {
                throw runtime_error(make_error("ftruncate() failed", n, errno));
            }
        }

        int prot = PROT_NONE;
        if (f & O_RDONLY)
            prot |= PROT_READ;
        if (f & O_RDWR)
            prot |= PROT_WRITE;

        off_t off = static_cast<off_t>(this->offset());
        mPointer = ::mmap(nullptr, size(), prot, MAP_SHARED, mFile, off);

        if (mPointer == nullptr)
            throw runtime_error(make_error("mmap() failed", n, errno));
        PSHM_ALLOCATION_TRACER_NOTE_POINTER("mmap() returned", mPointer);
    }

    posix_shm_object::~posix_shm_object()
    {
        if (mPointer != nullptr) {
            ::munmap(mPointer, size());
        }
        if (mFile > 0) {
            shm_unlink(name().c_str());
        }
    }

    void* posix_shm_object::get() const noexcept
    {
        return mPointer;
    }

} // namespace pshm
