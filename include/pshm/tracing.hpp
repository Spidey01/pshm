#ifndef PSHM_PSHM_TRACE__HPP
#define PSHM_PSHM_TRACE__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/allocation_tracer.hpp>
#include <pshm/config.hpp>

#if PSHM_TRACING

/**
 * @brief Declare a static member for tracing.
 * @param field_name the name of the field.
 */
#define PSHM_DECLARE_ALLOCATION_TRACER_BY_NAME(field_name) \
    static pshm::allocation_tracer field_name

/**
 * @brief Define the static member for tracing.
 * @param field_name the name of the field. You'll want this to be qualified, ala
 * @param tag the tag to pass to the allocation_tracer ctor.
 */
#define PSHM_DEFINE_ALLOCATION_TRACER_BY_NAME(field_name, tag) \
    pshm::allocation_tracer field_name(#tag);

/**
 * @brief This assumes that field_name is in a namespace that matches tag.
 *
 */
#define PSHM_DEFINE_ALLOCATION_TRACER_BY_NAMEANDTAG(field_name, tag) \
    PSHM_DEFINE_ALLOCATION_TRACER_BY_NAME(tag::field_name, tag)

/**
 * @brief Calls allocation_tracer::allocated().
 */
#define PSHM_ALLOCATION_TRACER_ALLOCATED_BY_NAME(field_name, name) \
    tracer_allocated(field_name, name);

/**
 * @brief Calls allocation_tracer::deallocated().
 */
#define PSHM_ALLOCATION_TRACER_DEALLOCATED_BY_NAME(field_name, name) \
    tracer_deallocated(field_name, name);

/**
 * @brief Calls allocation_tracer::note_pointer().
 */
#define PSHM_ALLOCATION_TRACER_NOTE_POINTER_BY_NAME(field_name, msg, name) \
    pshm::tracer_note_pointer(field_name, msg, name)

#else // !PSHM_TRACING

#define PSHM_DECLARE_ALLOCATION_TRACER_BY_NAME(field_name)                 /* Compiled without allocation_tracer. */
#define PSHM_DEFINE_ALLOCATION_TRACER_BY_NAME(field_name, tag)             /* Compiled without allocation_tracer */
#define PSHM_ALLOCATION_TRACER_ALLOCATED_BY_NAME(field_name, name)         /* Compiled without allocation_tracer. */
#define PSHM_ALLOCATION_TRACER_DEALLOCATED_BY_NAME(field_name, name)       /* Compiled without allocation_tracer. */
#define PSHM_ALLOCATION_TRACER_NOTE_POINTER_BY_NAME(field_name, msg, name) /* Compiled without allocation_tracer. */

#endif // !PSHM_TRACING

/**
 * @brief Default allocation_tracer field name.
 *
 * Expected to be used such that this is a field in the class, and the tag will
 * be the name of the class.
 */
#define PSHM_DEFAULT_ALLOCATION_TRACER sAllocationTracer

/**
 * @brief Declare a static member for tracing.
 *
 * If PSHM_TRACING is disabled this is a no-op.
 */
#define PSHM_DECLARE_ALLOCATION_TRACER() \
    PSHM_DECLARE_ALLOCATION_TRACER_BY_NAME(PSHM_DEFAULT_ALLOCATION_TRACER)

/**
 * @brief Define a static member for tracing.
 *
 * If PSHM_TRACING is disabled this is a no-op.
 */
#define PSHM_DEFINE_ALLOCATION_TRACER(tag) \
    PSHM_DEFINE_ALLOCATION_TRACER_BY_NAMEANDTAG(PSHM_DEFAULT_ALLOCATION_TRACER, tag)

/**
 * @brief Calls allocation_tracer::allocated().
 *
 * If PSHM_TRACING is disabled this is a no-op.
 */
#define PSHM_ALLOCATION_TRACER_ALLOCATED(name) \
    PSHM_ALLOCATION_TRACER_ALLOCATED_BY_NAME(PSHM_DEFAULT_ALLOCATION_TRACER, name)

/**
 * @brief Calls allocation_tracer::deallocated().
 *
 * If PSHM_TRACING is disabled this is a no-op.
 */
#define PSHM_ALLOCATION_TRACER_DEALLOCATED(name) \
    PSHM_ALLOCATION_TRACER_DEALLOCATED_BY_NAME(PSHM_DEFAULT_ALLOCATION_TRACER, name)

/**
 * @brief Calls allocation_tracer::note_pointer().
 *
 * If PSHM_TRACING is disabled this is a no-op.
 */
#define PSHM_ALLOCATION_TRACER_NOTE_POINTER(msg, name) \
    PSHM_ALLOCATION_TRACER_NOTE_POINTER_BY_NAME(PSHM_DEFAULT_ALLOCATION_TRACER, msg, name)

namespace pshm
{
    template <class T>
    void tracer_note_pointer(allocation_tracer& tracer, const std::string& msg, const T* ptr)
    {
        tracer.note_pointer(msg, ptr);
    }

    inline void tracer_allocated(allocation_tracer& tracer, const std::string& name)
    {
        tracer.allocated(name);
    }

    inline void tracer_deallocated(allocation_tracer& tracer, const std::string& name)
    {
        tracer.deallocated(name);
    }

    template <class... Args>
    void tracer_report(allocation_tracer& tracer, Args&&... args)
    {
        tracer.report(std::forward<Args>(args)...);
    }

} // namespace pshm

#endif // PSHM_PSHM_TRACE__HPP