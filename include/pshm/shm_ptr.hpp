#ifndef PSHM_SHM_PTR__HPP
#define PSHM_SHM_PTR__HPP
// SPDX-License-Identifier: Zlib
// Copyright 2022-Current, Terry M. Poulin.

#include <pshm/config.hpp>
#include <pshm/shm_object.hpp>
#include <pshm/stdcpp.hpp>

#if defined(__unix__)
#include <pshm/posix_shm_object.hpp>
#elif defined(_WIN32)
// TODO
#endif

namespace pshm
{
    /**
     * @brief Fancy pointer to shared memory.
     *
     * Using similar semantics to std::unique_ptr<>, shm_ptr provides a simple
     * interface to a shared memory object.
     *
     * @tparam T the type pointed to. Must be a trivial type. That basically
     * means if you can't use memcpy() it won't play nice in shared memory.
     *
     * @see std::unique_ptr<>.
     */
    template <class T, typename std::enable_if_t<std::is_trivial<T>::value, T>* = nullptr>
    class shm_ptr
    {
      public:
        using element_type = T;
        using pointer = element_type*;
        using flags_type = shm_object::flags_type;
        using offset_type = shm_object::offset_type;
        using size_type = shm_object::size_type;
        using string_type = shm_object::string_type;

        /**
         * @brief Create a shared memory object that refers nothing.
         *
         * Equivalent to shm_ptr(nullptr).
         */
        constexpr shm_ptr() noexcept
            : mObject(nullptr)
        {
        }

        /**
         * @brief Create a shared memory object that refers nothing.
         */
        constexpr shm_ptr(std::nullptr_t) noexcept
            : mObject(nullptr)
        {
        }

        /** @brief shm_ptr cannot be copied directly.
         */
        shm_ptr(const shm_ptr& other) noexcept = delete;

        /**
         * @brief Construct a new shm ptr object
         *
         * @param other cannibalize this and leave it nullptr.
         */
        shm_ptr(shm_ptr&& other) noexcept
        {
            mObject = std::move(other.mObject);
            other.mObject = nullptr;
        }

        /**
         * @brief Construct a new shm ptr object.
         *
         * @param name two objects with the same name refer to the same object
         * in shared memory. For portability this should begin with '/' on unix
         * platforms.
         *
         * @param flags the file control flags.
         *
         * @param offset the offset into the mapping to start.
         *
         * @see pshm::flags.
         * @see pshm::shm_object.
         */
        shm_ptr(const string_type& name, flags_type flags, offset_type offset)
            : mObject(make_shm_object(name, flags, sizeof(T), offset))
        {
        }

        /**
         * @brief Construct a new shm ptr object.
         *
         * Equivalent to shm_ptr(name, flags, 0).
         *
         * @param name two objects with the same name refer to the same object
         * in shared memory. For portability this should begin with '/' on unix
         * platforms.
         *
         * @param flags the file control flags.
         */
        shm_ptr(const string_type& name, flags_type flags)
            : mObject(make_shm_object(name, flags, sizeof(T), 0))
        {
        }

        /**
         * @brief Construct a new shm ptr object.
         *
         * Equivalent to shm_ptr(name, flags, 0) where flags is set to read, write, create.
         *
         * @param name two objects with the same name refer to the same object
         * in shared memory. For portability this should begin with '/' on unix
         * platforms.
         */
        shm_ptr(const string_type& name)
            : mObject(make_shm_object(name, flags::RDWR | flags::CREAT, sizeof(T), 0))
        {
        }

        /**
         * @brief Destroy the shm ptr object
         *
         * If multiple shm_ptr point to the same shared memory object, said
         * shared memory segment should persist until the last is destroyed.
         */
        ~shm_ptr()
        {
            if (mObject)
                delete mObject;
        }

        /**
         * @brief shm_ptr cannot be copy assigned directly.
         */
        shm_ptr& operator=(const shm_ptr& r) noexcept = delete;

        /**
         * @brief Move assignment.
         *
         * @param r cannibalize and leave it nullptr.
         * @returns *this.
         */
        shm_ptr& operator=(shm_ptr&& r) noexcept
        {
            if (this != &r) {
                mObject = std::move(r.mObject);
                r.mObject = nullptr;
            }
            return *this;
        }

        /**
         * @brief Assign the null pointer.
         *
         * If this shm_ptr referred to a shared memory object, it is deleted and
         * replaced with the nullptr.
         *
         * @returns *this.
         */
        shm_ptr& operator=(std::nullptr_t) noexcept
        {
            if (mObject)
                delete mObject;
            mObject = nullptr;
            return *this;
        }

        /**
         * @brief Swap two shm_ptr.
         *
         * @param other the target to swap.
         */
        void swap(shm_ptr& other) noexcept
        {
            std::swap(*this, other);
        }

        /**
         * @returns true when get() != nullptr.
         */
        explicit operator bool() const noexcept
        {
            return get() != nullptr;
        }

        /**
         * @brief Get the pointer.
         *
         * @returns a pointer to the managed object or nullptr if no object is
         * owned
         */
        pointer get() const noexcept
        {
            if (!mObject)
                return nullptr;
            return (pointer)mObject->get();
        }

        /**
         * @brief dereferences pointer to the managed object.
         *
         * @return the object owned by *this, equivalent to *get().
         *
         * @throws May throw if pointer has a throwing operator*.
         */
        typename std::add_lvalue_reference<T>::type operator*() const noexcept(noexcept(*std::declval<pointer>()))
        {
            return *get();
        }

        /**
         * @brief dereferences pointer to the managed object.
         *
         * @returns a pointer to the object owned by *this, i.e. get().
         */
        pointer operator->() const noexcept
        {
            return get();
        }

      private:
        shm_object* mObject;
    };
} // namespace pshm

/** @brief Inserts the value of the pointer stored in ptr into the output stream os.
 *
 * Equivalent to os << ptr.get().
 *
 * @param os std::basic_ostream to insert ptr into.
 * @param ptr the data to be inserted into os.
 * @returns os.
 */
template <class T, class U, class V>
std::basic_ostream<U, V>&
operator<<(std::basic_ostream<U, V>& os, const pshm::shm_ptr<T>& ptr)
{
    os << ptr.get();
    return os;
}

/** @brief Specializes the std::swap algorithm for shm_ptr.
 *
 * Swaps the contents of lhs and rhs.Calls lhs.swap(rhs).Reminder: std::swap()
 * relies on ADL :).
 *
 * @param lhs the pointer to call swap on.
 * @param rhs the target of the swap
.*/
template <class T>
void swap(pshm::shm_ptr<T>& lhs, pshm::shm_ptr<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class T, class U>
pshm::shm_ptr<T> static_pointer_cast(const pshm::shm_ptr<U>& r) noexcept
{
    auto p = static_cast<typename pshm::shm_ptr<T>::element_type*>(r.get());
    return pshm::shm_ptr<T>{r, p};
}

template <class T, class U>
pshm::shm_ptr<T> dynamic_pointer_cast(const pshm::shm_ptr<U>& r) noexcept
{
    if (auto p = dynamic_cast<typename pshm::shm_ptr<T>::element_type*>(r.get())) {
        return pshm::shm_ptr<T>{r, p};
    } else {
        return pshm::shm_ptr<T>{};
    }
}

template <class T, class U>
pshm::shm_ptr<T> const_pointer_cast(const pshm::shm_ptr<U>& r) noexcept
{
    auto p = const_cast<typename pshm::shm_ptr<T>::element_type*>(r.get());
    return pshm::shm_ptr<T>{r, p};
}

template <class T, class U>
pshm::shm_ptr<T> reinterpret_pointer_cast(const pshm::shm_ptr<U>& r) noexcept
{
    auto p = reinterpret_cast<typename pshm::shm_ptr<T>::element_type*>(r.get());
    return pshm::shm_ptr<T>{r, p};
}

/** @brief Specialization of std::hash for pshm::shm_ptr<>.
 */
template <typename T>
struct std::hash<pshm::shm_ptr<T>> {
    size_t operator()(const pshm::shm_ptr<T>& s) const noexcept
    {
        return hash<typename pshm::shm_ptr<T>::element_type*>()(s.get());
    }
};

#endif // PSHM_SHM_PTR__HPP