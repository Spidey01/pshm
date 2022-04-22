#ifndef PSHM__ALLOCATION_TRACER__HPP
#define PSHM__ALLOCATION_TRACER__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/config.hpp>
#include <pshm/stdcpp.hpp>

namespace pshm
{
    /**
     * @brief Trace allocations.
     *
     * The pshm library will compile out use of allocation_tracer when PSHM_TRACING is not defined.
     *
     * Put one of these as a static field of a class and call the methods when
     * you allocate or deallocate the memory you want to trace.
     *
     * Tracing consumes significant resources and is only intended for short
     * term debugging purposes. Running for a long period of operation with many
     * allocations and deallocations will leak memory, as records are maintained
     * for later reporting.
     *
     * The pshm test suite uses this to print a report at the end.
     */
    class allocation_tracer
    {
      public:
        /**
         * @brief Construct a new allocation tracer object
         *
         * @param tag top level tag to use for this trace.
         * @param output the output stream to use.
         */
        allocation_tracer(const std::string& tag, std::ostream* output)
            : mTag(tag)
            , mOutput(output)
        {
        }

        /**
         * @brief Construct a new allocation tracer with default output.
         *
         * When PSHM_TRACING is enabled this is equal to allocation_tracer(tag,
         * &std::cout). When PSHM_TRACING is disabled this is equal to
         * allocation_tracer(tag, nullptr).
         *
         * @param tag top level tag to use for this trace.
         */
        allocation_tracer(const std::string& tag)
            : mTag(tag)
#if PSHM_TRACING
            , mOutput(&std::cout)
#else
            , mOutput(nullptr)
#endif
        {
        }

        /**
         * @brief Destroy the allocation tracer object
         *
         * If PSHM_TRACING is enabled then report() will be called. Useful for
         * dumping the info on program exit.
         */
        ~allocation_tracer()
        {
#if PSHM_TRACING
            report();
#endif
        }

        /**
         * @brief Record that name was allocated.
         *
         * A simple count is made starting from 1. If the same name is allocated
         * again the count will be incremented.
         *
         * @param name name of the record.
         */
        void allocated(const std::string& name)
        {
            int64_t& c = get_and_inc(name);
            if (mOutput)
                *mOutput << mTag << ": allocated: " << name << " count: " << c << std::endl;
        }

        /**
         * @brief Record that name was deallocated.
         *
         * Decrements the record's count.
         *
         * @param name name of the record.
         */
        void deallocated(const std::string& name)
        {
            int64_t& c = get_and_dec(name);
            if (mOutput)
                *mOutput << mTag << ": deallocated: " << name << " count: " << c << std::endl;
        }

        /**
         * @brief Note the address of a pointer.
         *
         * Useful for cases where you want the pointer address to be noted while
         * tracing.
         *
         * @tparam T some type that can be output to an ostream.
         * @param msg message to log.
         * @param ptr address to log.
         */
        template <class T>
        void note_pointer(const std::string& msg, T* ptr)
        {
            if (mOutput)
                *mOutput << mTag << ": " << msg << ": " << ptr << std::endl;
        }

        /**
         * @brief Print a report to the output.
         */
        void report()
        {
            if (mOutput)
                report(*mOutput);
        }

        /**
         * @brief Print a report to the specified output.
         *
         * Prints this allocation tracer's tag followed by an indented list of
         * the allocations and their current counts.
         *
         * @param output the ostream to print to.
         */
        void report(std::ostream& output)
        {
            output << "allocation_tracer report: tag: " << mTag << " records: " << mMap.size() << std::endl;
            for (const auto& pair : mMap) {
                output << '\t' << pair.first << ": count: " << pair.second << std::endl;
            }
        }

      private:
        std::string mTag;
        std::ostream* mOutput;
        std::map<std::string, int64_t> mMap;

        /**
         * @brief Get the count.
         *
         * @param name the record being counted.
         *
         * @returns a reference to the recorded count. If none exists it will be
         * set to 0.
         */
        int64_t& count(const std::string& name)
        {
            int64_t* c;
            if (mMap.find(name) == mMap.end()) {
                c = &mMap[name];
                *c = 0;
            } else {
                c = &mMap[name];
            }
            return *c;
        }

        /**
         * @brief Get and increment the counter.
         *
         * @param name the record being counted.
         * @returns the incremented count.
         */
        int64_t& get_and_inc(const std::string& name)
        {
            int64_t& c = count(name);
            c += 1;
            return c;
        }

        /**
         * @brief Get and decrement the counter.
         *
         * @param name the record being counted.
         * @returns the decremented count.
         */
        int64_t& get_and_dec(const std::string& name)
        {
            int64_t& c = count(name);
            c -= 1;
            return c;
        }
    };
} // namespace pshm

#endif // PSHM__ALLOCATION_TRACER__HPP