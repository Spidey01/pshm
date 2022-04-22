#ifndef PSHM_SHM_OBJECT__HPP
#define PSHM_SHM_OBJECT__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/config.hpp>
#include <pshm/flags.hpp>
#include <pshm/stdcpp.hpp>
#include <pshm/tracing.hpp>

namespace pshm
{
    /**
     * @brief Represents an object in shared memory.
     *
     */
    class shm_object
    {
      public:
        using flags_type = pshm::flags_t;
        using offset_type = std::streamsize;
        using size_type = size_t;
        using string_type = std::string;

        /**
         * @brief Construct a new shm object object.
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
        shm_object(const string_type& name, flags_type flags, size_type length, offset_type offset);

        virtual ~shm_object();

        /**
         * @brief Get the name of this object.
         *
         * @returns the shared memory object name.
         */
        string_type name() const noexcept;

        flags_type flags() const noexcept;
        size_type size() const noexcept;
        offset_type offset() const noexcept;

        virtual void* get() const noexcept = 0;

      protected:
        /**
         * @brief Builds a string error message from mTag.
         *
         * @param msg the message to use.
         */
        string_type make_error(const string_type& msg) const;

        /**
         * @brief Add std::strerror() to the error code.
         *
         * @param msg the message to use.
         *
         * @param error passed to std::sterror()
         */
        string_type make_error(const string_type& msg, int error) const;

        /**
         * @brief Make fancy error message.
         *
         * @param msg the message body to use.
         *
         * @param name the shared memory object name, e.g. mName.
         *
         * @param error passed to std::sterror()
         */
        string_type make_error(const string_type& msg, const string_type& name, int error) const;

      private:
        PSHM_DECLARE_ALLOCATION_TRACER();
        string_type mName;
        flags_type mFlags;
        size_type mSize;
        offset_type mOffset;
        string_type mTag;
    };

} // namespace pshm

#endif // PSHM_SHM_OBJECT__HPP