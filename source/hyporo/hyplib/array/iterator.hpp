#pragma once

#include <iterator>


namespace hpr
{

template <typename Type, typename Category = std::forward_iterator_tag>
class Iterator
{
public:
    using iterator_category = Category;
    using difference_type = std::ptrdiff_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator;
    using const_pointer = Type const*;
    using const_reference = Type const&;
    using const_iterator = Iterator const;

protected:
    pointer p_ptr;

public:

    Iterator() :
        p_ptr {nullptr}
    {}

    Iterator(pointer ptr) :
        p_ptr {ptr}
    {}

    reference operator*()
    {
        return *p_ptr;
    }

    const_reference operator*() const
    {
        return *p_ptr;
    }

    pointer operator->()
    {
        return p_ptr;
    }

    const_pointer operator->() const
    {
        return p_ptr;
    }

    iterator& operator++()
    {
        p_ptr++;
        return *this;
    }

    const_iterator& operator++() const
    {
        p_ptr++;
        return *this;
    }

    iterator operator++(int)
    {
        iterator temp {*this};
        ++(*this);
        return temp;
    }

    iterator operator+(int value)
    {
        iterator temp {*this};
        temp.p_ptr += value;
        return temp;
    }

    friend
    bool operator==(const iterator& lhs, const iterator& rhs)
    {
        return lhs.p_ptr == rhs.p_ptr;
    }

    friend
    bool operator!=(const iterator& lhs, const iterator& rhs)
    {
        return lhs.p_ptr != rhs.p_ptr;
    }
};

}
