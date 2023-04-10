#pragma once

#include <hpr/containers/array/iterator.hpp>
#include <hpr/exception.hpp>
#include <hpr/math/integer.hpp>

namespace hpr
{

// forward declaration

template <typename T>
class Sequence;

// type traits

template <typename T>
struct is_sequence : public std::false_type
{};

template <typename T>
struct is_sequence<Sequence<T>> : public std::true_type
{};

// concepts

template <typename Type>
concept IsSequence = is_sequence<Type>::value;

// class definition

template <typename T>
class Sequence
{

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

protected:

    size_type p_size;
    size_type p_capacity;
    pointer p_start;

protected:

    constexpr
    Sequence(size_type size, size_type capacity, pointer start) :
        p_size {size},
        p_capacity {capacity},
        p_start {start}
    {}

    constexpr explicit
    Sequence(size_type size, size_type capacity) :
        p_size {size},
        p_capacity {capacity},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {}

public:

    friend constexpr
    void swap(Sequence& main, Sequence& other) noexcept
    {
        using std::swap;
        swap(main.p_size, other.p_size);
        swap(main.p_capacity, other.p_capacity);
        swap(main.p_start, other.p_start);
    }

    constexpr
    Sequence() noexcept :
        p_size {0},
        p_capacity {1},
        p_start {new value_type[p_capacity]}
    {}

    constexpr
    Sequence(const Sequence& seq) noexcept :
        p_size {seq.p_size},
        p_capacity {seq.p_capacity},
        p_start {p_capacity ? new value_type[seq.p_capacity] : nullptr}
    {
        std::copy(seq.p_start, seq.p_start + seq.p_size, p_start);
    }

    //! Move constructor
    constexpr
    Sequence(Sequence&& seq) noexcept :
        Sequence {}
    {
        swap(*this, seq);
    }

    template <typename Iter>
    constexpr
    Sequence(Iter start, Iter end) :
        p_size {static_cast<size_type>(std::distance(start, end))},
        p_capacity {p_size},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        std::copy(start, end, p_start);
    }

    constexpr
    Sequence(iterator start, iterator end) :
        p_size {static_cast<size_type>(std::distance(start, end))},
        p_capacity {p_size},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        std::copy(start, end, p_start);
    }

    constexpr
    Sequence(const_iterator start, const_iterator end) :
        p_size {static_cast<size_type>(std::distance(start, end))},
        p_capacity {p_size},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        std::copy(start, end, p_start);
    }

    constexpr
    Sequence(iterator start, iterator end, size_type capacity) :
        p_size {static_cast<size_type>(std::distance(start, end))},
        p_capacity {capacity},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        std::copy(start, end, p_start);
    }

    constexpr
    Sequence(const_iterator start, const_iterator end, size_type capacity) :
        p_size {static_cast<size_type>(std::distance(start, end))},
        p_capacity {capacity},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        std::copy(start, end, p_start);
    }

    /*constexpr
    Sequence(std::initializer_list<value_type> list) :
        Sequence {list.begin(), list.end()}
    {}*/

    constexpr
    Sequence(size_type size, value_type value) noexcept:
        p_size {size},
        p_capacity {size},
        p_start {p_capacity ? new value_type[p_capacity] : nullptr}
    {
        for (auto n = 0; n < size; ++n)
            *(p_start + n) = value;
    }

    constexpr
    Sequence& operator=(Sequence other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    constexpr
    Sequence& operator=(Sequence&& seq) noexcept
    {
        swap(*this, seq);
        return *this;
    }

    virtual
    ~Sequence()
    {
        delete[] p_start;
    }


    // Member functions

    virtual constexpr
    iterator begin()
    {
        return iterator(p_start);
    }

    [[nodiscard]] virtual constexpr
    const_iterator begin() const
    {
        return const_iterator(p_start);
    }

    virtual constexpr
    iterator end()
    {
        return iterator(p_start + p_size);
    }

    virtual constexpr
    const_iterator end() const
    {
        return const_iterator(p_start + p_size);
    }

    virtual constexpr
    iterator storage_end()
    {
        return iterator(p_start + p_capacity);
    }

    virtual constexpr
    const_iterator storage_end() const
    {
        return const_iterator(p_start + p_capacity);
    }

    [[nodiscard]] virtual constexpr
    size_type size() const noexcept
    {
        return p_size;
    }

    virtual constexpr
    reference operator[](size_type n)
    {
        if (n >= size())
            throw hpr::OutOfRange();
        return *(p_start + n);
    }

    virtual constexpr
    const_reference operator[](size_type n) const
    {
        if (n >= size())
            throw hpr::OutOfRange();
        return *(p_start + n);
    }

    virtual constexpr
    reference at(size_type n)
    {
        return operator[](n);
    }

    virtual constexpr
    const_reference at(size_type n) const
    {
        return operator[](n);
    }

    virtual constexpr
    reference front()
    {
        return *begin();
    }

    virtual constexpr
    const_reference front() const
    {
        return *begin();
    }

    virtual constexpr
    reference back()
    {
        return *(p_start + p_size - 1);
    }

    virtual constexpr
    const_reference back() const
    {
        return *(p_start + p_size - 1);
    }

    virtual constexpr
    pointer data()
    {
        return p_start;
    }

    [[nodiscard]] virtual constexpr
    const_pointer data() const
    {
        return p_start;
    }

    // Friend functions

    friend
    bool operator==(const Sequence& lhs, const Sequence& rhs)
    {
        for (auto n = 0; n < lhs.size(); ++n)
            if (lhs[n] != rhs[n])
                return false;
        return true;
    }

    friend
    Sequence operator+(const Sequence& lhs, const Sequence& rhs)
    {
        Sequence seq {rhs.size() + lhs.size(), {}};
        for (auto n = 0; n < rhs.size(); ++n)
        {
            seq[n] = rhs[n];
            seq[n + rhs.size()] = lhs[n];
        }
        return seq;
    }
};

}