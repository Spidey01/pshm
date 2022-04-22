#ifndef PSHM_POSIX_SHM_OBJECT__HPP
#define PSHM_POSIX_SHM_OBJECT__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/shm_object.hpp>

namespace pshm
{
    /**
     * @brief Implements shm_object using the POSIX shared memory API.
     */
    class posix_shm_object
        : public shm_object
    {
      public:
        /**
         * @brief Construct a new shm object object.
         *
         * Effectively does shm_open(), mmap(), and shm_close().
         *
         * @param name two objects with the same name refer to the same object
         * in shared memory. For portability this should begin with '/' on unix
         * platforms.
         *
         * @param flags the file control flags.
         *
         * @param length the length of the mapping starting at offset.
         *
         * @param offset the offset into the mapping to start.
         *
         * @see pshm::flags.
         * @see pshm::shm_ptr.
         */
        posix_shm_object(const string_type& name, flags_type flags, size_type length, offset_type offset);

        /**
         * @brief Destroy the shm object unix object.
         *
         * Effectively calls munmap() on the pointer.
         */
        virtual ~posix_shm_object();

        /** @returns the underlaying pointer.
         */
        void* get() const noexcept override;

      private:
        PSHM_DECLARE_ALLOCATION_TRACER();
        void* mPointer;
        int mFile;
    };

    template <class... Args>
    shm_object* make_shm_object(Args&&... args)
    {
        return new posix_shm_object(std::forward<Args>(args)...);
    }

} // namespace pshm

#endif // PSHM_POSIX_SHM_OBJECT__HPP