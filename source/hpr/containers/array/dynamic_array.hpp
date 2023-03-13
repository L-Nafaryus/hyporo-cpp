#pragma once

#include <hpr/containers/array/sequence.hpp>

#include <functional>
#include <limits>


namespace hpr
{

// forward declaration

template <typename T>
class DynamicArray;

// type traits

template <typename T>
struct is_sequence<DynamicArray<T>> : public std::true_type
{};

// aliases

template <typename T>
using darray = DynamicArray<T>;

// class definition

template <typename T>
class DynamicArray : public Sequence<T>
{

    using base = Sequence<T>;

public:

    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using size_type = Size;
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<T>;
    using const_pointer = T const*;
    using const_reference = T const&;
    using const_iterator = Iterator<const T>;

public:

    friend constexpr
    void swap(DynamicArray& main, DynamicArray& other) noexcept
    {
        using std::swap;
        swap(static_cast<base&>(main), static_cast<base&>(other));
    }

    constexpr
    DynamicArray() noexcept :
        base {}
    {}

    constexpr explicit
    DynamicArray(const base& b) noexcept :
        base {b}
    {}

    constexpr
    DynamicArray(const DynamicArray& arr) noexcept :
        base {static_cast<base>(arr)}
    {}

    //! Move constructor
    constexpr
    DynamicArray(DynamicArray&& arr) noexcept :
        base {std::forward<base>(static_cast<base>(arr))}
    {}

    template <typename Iter>
    constexpr
    DynamicArray(Iter start, Iter end) :
        base {start, end}
    {}

    constexpr
    DynamicArray(typename base::iterator start, typename base::iterator end) :
        base {start, end}
    {}

    constexpr
    DynamicArray(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end}
    {}

    constexpr
    DynamicArray(typename base::iterator start, typename base::iterator end, size_type capacity) :
        base {start, end, capacity}
    {}

    constexpr
    DynamicArray(typename base::const_iterator start, typename base::const_iterator end, size_type capacity) :
        base {start, end, capacity}
    {}

    constexpr
    DynamicArray(std::initializer_list<value_type> list) :
        base {list.begin(), list.end()}
    {}

    constexpr
    DynamicArray(size_type size, value_type value) noexcept :
        base {size, value}
    {}

    constexpr
    DynamicArray& operator=(DynamicArray other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    constexpr
    DynamicArray& operator=(DynamicArray&& arr) noexcept
    {
        swap(*this, arr);
        return *this;
    }

    virtual
    ~DynamicArray() = default;

    // Member functions

    [[nodiscard]] virtual constexpr
    size_type capacity() const noexcept
    {
        return base::p_capacity;
    }

    [[nodiscard]] virtual constexpr
    bool is_empty() const
    {
        return base::begin() == base::end();
    }

    virtual constexpr
    iterator storage_end()
    {
        return iterator(base::p_start + base::p_capacity);
    }

    virtual constexpr
    const_iterator storage_end() const
    {
        return const_iterator(base::p_start + base::p_capacity);
    }

    virtual constexpr
    void resize(size_type newCapacity)
    {
        if (newCapacity == base::p_capacity)
            return;

        if (std::numeric_limits<size_type>::max() - base::size() < newCapacity)
            throw hpr::LengthError("Invalid capacity value passed");

        if (newCapacity > base::p_capacity)
        {
            DynamicArray tmp {base::begin(), base::end(), newCapacity};
            swap(*this, tmp);
        }
        else
        {
            if (newCapacity >= base::p_size)
            {
                DynamicArray tmp {base::begin(), base::end()};
                swap(*this, tmp);
            }
            else
            {
                DynamicArray tmp {base::begin(), base::begin() + newCapacity};
                swap(*this, tmp);
            }
        }
    }

    virtual constexpr
    void push(const value_type& val)
    {
        if (base::end() == storage_end())
            resize(base::p_capacity * 2);
        *base::end() = val;
        ++base::p_size;
    }

    virtual constexpr
    void push(value_type&& val)
    {
        if (base::end() == storage_end())
            resize(base::p_capacity * 2);
        *base::end() = std::move(val);
        ++base::p_size;
    }

    virtual constexpr
    void push(const DynamicArray<T>& arr)
    {
        for (const value_type& el : arr)
            push(el);
    }

    virtual constexpr
    value_type pop()
    {
        if (is_empty())
            throw hpr::LengthError("Cannot pop element from empty array");
        value_type val = base::back();
        std::destroy_at(base::p_start + base::p_size);
        --base::p_size;
        return val;
    }

    virtual constexpr
    void insert(size_type position, const value_type& val)
    {
        if (base::end() == storage_end())
            resize(base::p_capacity * 2);
        for (size_type n = base::p_size; n > position; --n)
            *(base::p_start + n) = std::move(*(base::p_start + n - 1));
        *(base::p_start + position) = val;
        ++base::p_size;
    }

    virtual constexpr
    void insert(size_type position, value_type&& val)
    {
        if (base::end() == storage_end())
            resize(base::p_capacity * 2);
        for (size_type n = base::p_size; n > position; --n)
            *(base::p_start + n) = std::move(*(base::p_start + n - 1));
        *(base::p_start + position) = std::move(val);
        ++base::p_size;
    }

    virtual constexpr
    void remove(size_type position)
    {
        for (size_type n = position; n < base::p_size - 1; ++n)
            *(base::p_start + n) = std::move(*(base::p_start + n + 1));
        std::destroy_at(base::p_start + base::p_size);
        --base::p_size;
    }

    virtual constexpr
    void remove(iterator position)
    {
        if (position + 1 != base::end())
            std::copy(position + 1, base::end(), position);
        std::destroy_at(base::p_start + base::p_size);
        --base::p_size;
    }

    virtual constexpr
    void remove(const std::function<bool(value_type)>& condition)
    {
        size_type newSize = base::p_size;
        for (size_type offset = 0; offset < newSize; ++offset)
        {
            if (condition(*(base::p_start + offset)))
            {
                for (size_type n = offset; n < newSize; ++n)
                    *(base::p_start + n) = std::move(*(base::p_start + n + 1));

                --newSize;
                --offset;
            }
        }
        base::p_size = newSize;
    }

    virtual constexpr
    void clear()
    {
        delete[] base::p_start;
        base::p_size = 0;
        base::p_capacity = 1;
        base::p_start = new value_type[base::p_capacity];
    }

    DynamicArray slice(iterator start, iterator end)
    {
        return DynamicArray {start, end};
    }

};


}