#ifndef PSHM_SHM_OBJECT_NATIVE__HPP
#define PSHM_SHM_OBJECT_NATIVE__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/shm_object.hpp>

#if defined(__unix__)
#include <pshm/posix_shm_object.hpp>
#elif defined(_WIN32)
#include <pshm/win32_shm_object.hpp>
#else
#error shm_object not implemented for this platform
#endif

namespace pshm
{
    /**
     * @brief Type definition for the native shm_object implementation.
     */
#if defined(__unix__)
    using shm_object_native = pshm::posix_shm_object;
#elif defined(_WIN32)
    using shm_object_native = pshm::win32_shm_object;
#endif

    /**
     * @brief Allocate a new shm_object.
     * 
     * @param args forwarded to shm_object.
     * 
     * @returns a pointer to the new shm_object using the native implementation.
     */
    template <class... Args>
    shm_object* make_shm_object(Args&&... args)
    {
        return new shm_object_native(std::forward<Args>(args)...);
    }

} // namespace pshm

#endif // PSHM_SHM_OBJECT_NATIVE__HPP