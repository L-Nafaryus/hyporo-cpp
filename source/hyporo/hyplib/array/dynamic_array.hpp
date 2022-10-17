#pragma once

#include "iterator.hpp"
#include <limits>

namespace hpr
{

template <typename Type>
class DynamicArray
{

public:

    using difference_type = std::ptrdiff_t;
    using value_type = Type;
    using size_type = size_t;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator<Type>;
    using const_pointer = Type const*;
    using const_reference = Type const&;
    using const_iterator = Iterator<Type> const;

protected:

    size_type p_size;
    size_type p_capacity;
    pointer p_start;
    pointer p_end;
    pointer p_storage_end;

public:

    inline
    DynamicArray() :
        p_size {0},
        p_capacity {1},
        p_start {new value_type[p_capacity]},
        p_end {p_start},
        p_storage_end {p_end + p_capacity}
    {}

    inline
    DynamicArray(const DynamicArray& arr) :
        p_size {arr.p_size},
        p_capacity {arr.p_capacity},
        p_start {new value_type[arr.p_size]},
        p_end {p_start + p_size},
        p_storage_end {p_start + p_capacity}
    {
        std::copy(arr.p_start, arr.p_end, p_start);
    }

    //! Move constructor
    inline
    DynamicArray(DynamicArray&& arr) noexcept :
        DynamicArray {}
    {
        swap(*this, arr);
    }

    inline
    DynamicArray(std::initializer_list<value_type> list) :
        p_size {list.size()},
        p_capacity {list.size()},
        p_start {new value_type[p_capacity]},
        p_end {p_start + p_size},
        p_storage_end {p_start + p_capacity}
    {
        std::copy(list.begin(), list.end(), p_start);
    }

    inline
    DynamicArray& operator=(DynamicArray&& arr) noexcept
    {
        swap(*this, arr);
        return *this;
    }

    virtual
    ~DynamicArray()
    {
        //std::destroy(p_start, p_end);
        delete[] p_start;
    }

    // Member functions

    virtual
    iterator begin()
    {
        return iterator(p_start);
    }

    [[nodiscard]] virtual
    const_iterator begin() const
    {
        return const_iterator(p_start);
    }

    virtual
    iterator end()
    {
        return iterator(p_end);
    }

    virtual
    const_iterator end() const
    {
        return const_iterator(p_end);
    }


    [[nodiscard]] virtual
    size_type size() const
    {
        return size_type(p_end - p_start);
    }

    [[nodiscard]] virtual
    size_type capacity() const
    {
        return size_type(p_storage_end - p_start);
    }

    [[nodiscard]] virtual
    bool is_empty() const
    {
        return begin() == end();
    }

    virtual
    reference operator[](size_type n)
    {
        return *(p_start + n);
    }

    virtual
    const_reference operator[](size_type n) const
    {
        return *(p_start + n);
    }

    virtual
    reference front()
    {
        return *p_start;
    }

    virtual
    reference back()
    {
        return *(p_end - 1);
    }

    virtual
    pointer data()
    {
        return p_start;
    }

    [[nodiscard]] virtual
    const_pointer data() const
    {
        return p_start;
    }

    virtual
    void resize(size_type newCapacity)
    {
        if (newCapacity == p_capacity)
            return;
        if (std::numeric_limits<size_type>::max() - size() < newCapacity)
            throw std::length_error("Wrong capacity value passed (possibly negative)");

        pointer newStart = new value_type[newCapacity];

        if (newCapacity > p_capacity)
        {
            std::move(p_start, p_end, newStart);
        }
        else if (newCapacity < p_capacity)
        {
            if (newCapacity < p_size) {
                std::move(p_start, p_start + newCapacity, newStart);
                p_size = newCapacity;
            }
            else
            {
                std::move(p_start, p_end, newStart);
            }
        }

        delete[] p_start;
        std::swap(p_start, newStart);
        p_capacity = newCapacity;
        p_end = p_start + p_size;
        p_storage_end = p_start + p_capacity;
    }

    virtual
    void push(const value_type& val)
    {
        if (p_end == p_storage_end)
            resize(p_capacity * 2);
        *p_end = val;
        ++p_end;
        ++p_size;
    }

    virtual
    void push(value_type&& val)
    {
        if (p_end == p_storage_end)
            resize(p_capacity * 2);
        *p_end = std::move(val);
        ++p_end;
        ++p_size;
    }

    virtual
    value_type pop()
    {
        if (is_empty())
            throw std::length_error("Cannot pop element from empty array");
        value_type val = back();
        std::destroy_at(p_end);
        --p_end;
        --p_size;
        return val;
    }

    virtual
    void insert(size_type position, const value_type& val)
    {
        if (p_end == p_storage_end)
            resize(p_capacity * 2);
        for (size_type n = p_size; n > position; --n)
            *(p_start + n) = std::move(*(p_start + n - 1));
        *(p_start + position) = val;
        ++p_size;
        ++p_end;
    }

    virtual
    void insert(size_type position, value_type&& val)
    {
        if (p_end == p_storage_end)
            resize(p_capacity * 2);
        for (size_type n = p_size; n > position; --n)
            *(p_start + n) = std::move(*(p_start + n - 1));
        *(p_start + position) = std::move(val);
        ++p_size;
        ++p_end;
    }

    // Friend functions

    friend
    void swap(DynamicArray& lhs, DynamicArray& rhs)
    {
        std::swap(lhs.p_size, rhs.p_size);
        std::swap(lhs.p_capacity, rhs.p_capacity);
        std::swap(lhs.p_start, rhs.p_start);
        std::swap(lhs.p_end, rhs.p_end);
        std::swap(lhs.p_storage_end, rhs.p_storage_end);
    }
};

}