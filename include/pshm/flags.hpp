#ifndef PSHM_FLAGS__HPP
#define PSHM_FLAGS__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

namespace pshm
{
    typedef int flags_t;

    /**
     * @brief Flags for pshm.
     * 
     * When possible these values serve the same purpose as the O_* constants
     * from the POSIX fcntl.h header. Internally these are translated based on
     * the native platform.
     * 
     * For portability reasons: you should not assume these are the same values
     * match fcntl.h or any similar system header. Only that the concepts map.
     * E.g., Using RDONLY implies we should use O_RDONLY on Unix platforms, and
     * comparable values on non-Unix platforms.
     */
    class flags
    {
      public:
        /**
         * Open in read only mode.
         */
        static constexpr flags_t RDONLY = 00000000;

        /**
         * Open in read and write mode.
         */
        static constexpr flags_t RDWR = 00000002;

        /**
         * Create if doesn't exist.
         */
        static constexpr flags_t CREAT = 00000100;

        /**
         * If exists and CREATE set then generate an error.
         */
        static constexpr flags_t EXCL = 00000200;

        /**
         * If exists then truncate to zero bytes.
         */
        static constexpr flags_t TRUNC = 00001000;
    };

} // namespace pshm

#endif // PSHM_FLAGS__HPP