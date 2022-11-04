#pragma once

#include "iterator.hpp"

#include <limits>
#include <functional>


namespace hpr
{

template <typename Type, size_t Size>
class StaticArray
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
    using const_iterator = Iterator<const Type>;

protected:

    const size_type p_size;
    pointer p_start;
    pointer p_end;

protected:

    inline
    StaticArray(size_type size, pointer start, pointer end) :
        p_size {size},
        p_start {start},
        p_end {end}
    {}

public:

    inline
    StaticArray() :
            p_size {Size},
            p_start {new value_type[p_size] {}},
            p_end {p_start + p_size}
    {}

    inline
    StaticArray(const StaticArray& arr) :
            p_size {arr.p_size},
            p_start {new value_type[arr.p_size]},
            p_end {p_start + p_size}
    {
        std::copy(arr.p_start, arr.p_end, p_start);
    }

    //! Move constructor
    inline
    StaticArray(StaticArray&& arr) noexcept :
        StaticArray {}
    {
        swap(*this, arr);
    }

    inline
    StaticArray(const_iterator start, const_iterator end) :
        p_size {Size},
        p_start {new value_type[p_size]},
        p_end {p_start + p_size}
    {
        std::copy(start, end, p_start);
    }

    inline
    StaticArray(iterator start, iterator end) :
        p_size {Size},
        p_start {new value_type[p_size]},
        p_end {p_start + p_size}
    {
        std::copy(start, end, p_start);
    }

    inline
    StaticArray(std::initializer_list<value_type> list) :
        StaticArray {list.begin(), list.end()}
    {}

    template <std::convertible_to<value_type>... Args>
    inline
    StaticArray(const value_type& v, const Args& ...args) :
        StaticArray {std::initializer_list<value_type>({std::forward<value_type>(v),
            std::forward<value_type>(static_cast<value_type>(args))...})}
    {
        static_assert(1 + sizeof...(args) == Size, "Number of arguments must be equal to size of array");
    }

    template <std::convertible_to<value_type>... Args>
    inline
    StaticArray(value_type&& v, Args&& ...args) :
        StaticArray {std::initializer_list<value_type>({std::forward<value_type>(v),
            std::forward<value_type>(static_cast<value_type>(args))...})}
    {
        static_assert(1 + sizeof...(args) == Size, "Number of arguments must be equal to size of array");
    }

    template <size_type SubSize, std::convertible_to<value_type>... Args>
    inline
    StaticArray(const StaticArray<value_type, SubSize>& subarr, const value_type& v, const Args& ...args) :
        p_size {Size},
        p_start {new value_type[p_size]},
        p_end {p_start + p_size}
    {
        static_assert(SubSize + 1 + sizeof...(args) == Size,
            "Number of arguments and sub-array elements must be equal to size of main array");
        std::initializer_list<value_type> list {v, static_cast<value_type>(args)...};
        std::copy(subarr.begin(), subarr.end(), begin());
        size_type pos {subarr.size()};
        for (auto& val : list)
            (*this)[pos++] = val;
    }

    inline
    StaticArray& operator=(const StaticArray& vs) noexcept
    {
        std::copy(vs.begin(), vs.end(), begin());
        return *this;
    }

    inline
    StaticArray& operator=(StaticArray&& arr) noexcept
    {
        swap(*this, arr);
        return *this;
    }

    virtual
    ~StaticArray()
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

    // Friend functions

    friend
    void swap(StaticArray& lhs, StaticArray& rhs)
    {
        std::swap(lhs.p_start, rhs.p_start);
        std::swap(lhs.p_end, rhs.p_end);
    }

    friend
    bool operator==(const StaticArray& lhs, const StaticArray& rhs)
    {
        for (auto n = 0; n < Size; ++n)
            if (lhs[n] != rhs[n])
                return false;
        return true;
    }
};

}
