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
         * @brief shm_object is not copyable by value.
         * 
         * While the base class interface has easily copied fields: the desired
         * semantics for shm_objects implementations are non-copyable.
         * 
         * @param o if you need a real copy: use pointers.
         */
        shm_object(const shm_object& o) = delete;

        /**
         * @brief shm_object is not copyable by value.
         * 
         * While the base class interface has easily copied fields: the desired
         * semantics for shm_objects implementations are non-copyable.
         * 
         * @param o if you need a real copy: use pointers.
         */
        shm_object& operator=(const shm_object& o) noexcept = delete;

        /**
         * @brief Move construction.
         * 
         * Implementations of shm_object are expected to provide a move
         * constructor that leaves the resource as nullptr or to delete their
         * move constructor.
         * 
         * Currently move construction does not erase the shm_object fields
         * (name(), etc) because shm_object() has no default constructor. This
         * may change in the future.
         * 
         * @param r the resource to cannibalize. Implementations should transfer
         * the resource in the same spirit as a std::unique_ptr.
         */
        shm_object(shm_object&& r) = default;


        /**
         * @brief Move assignment.
         * 
         * Implementations of shm_object are expected to provide a move
         * constructor that leaves the resource as nullptr or to delete their
         * move constructor.
         * 
         * Currently move assignment does not erase the shm_object fields
         * (name(), etc) because shm_object() has no default constructor. This
         * may change in the future.
         * 
         * @param r the resource to cannibalize. Implementations should transfer
         * the resource in the same spirit as a std::unique_ptr.
         */
        shm_object& operator=(shm_object&& r) noexcept = default;

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