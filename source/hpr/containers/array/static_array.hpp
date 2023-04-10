#pragma once

#include <hpr/containers/array/sequence.hpp>


namespace hpr
{

// forward declaration

template <typename T, Size S>
requires (S > 0)
class StaticArray;

// type traits

template <typename T, Size S>
struct is_sequence<StaticArray<T, S>> : public std::true_type
{};

// aliases

template <typename T, Size S>
using sarray = StaticArray<T, S>;

// class definition

template <typename T, Size S>
requires (S > 0)
class StaticArray : public Sequence<T>
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
    void swap(StaticArray& main, StaticArray& other) noexcept
    {
        using std::swap;
        swap(static_cast<base&>(main), static_cast<base&>(other));
    }

    //! Default constructor
    constexpr
    StaticArray() noexcept :
        base {S, S, new value_type[S] {}}
    {}

    constexpr explicit
    StaticArray(const base& b) noexcept :
        base {b}
    {}

    //! Copy constructor
    constexpr
    StaticArray(const StaticArray& arr) noexcept :
        base {static_cast<base>(arr)}
    {}

    //! Move constructor
    constexpr
    StaticArray(StaticArray&& arr) noexcept :
        base {std::forward<base>(static_cast<base>(arr))}
    {}

    constexpr
    StaticArray(typename base::iterator start, typename base::iterator end) :
        base {start, end}
    {}

    constexpr
    StaticArray(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end}
    {}

    constexpr
    StaticArray(typename base::iterator start, typename base::iterator end, size_type capacity) :
        base {start, end, capacity}
    {}

    constexpr
    StaticArray(typename base::const_iterator start, typename base::const_iterator end, size_type capacity) :
        base {start, end, capacity}
    {}

    constexpr
    StaticArray(std::initializer_list<value_type> list) :
        base {list.begin(), list.end()}
    {}

    template <std::convertible_to<value_type>... Args>
    constexpr explicit
    StaticArray(const value_type& v, const Args& ...args)
    requires (1 + sizeof...(args) == S) :
        StaticArray {std::initializer_list<value_type>({v, static_cast<value_type>(args)...})}
    {}

    template <std::convertible_to<value_type>... Args>
    constexpr explicit
    StaticArray(value_type&& v, Args&& ...args)
    requires (1 + sizeof...(args) == S) :
        StaticArray {std::initializer_list<value_type>({std::forward<value_type>(v), std::forward<value_type>(static_cast<value_type>(args))...})}
    {}

    template <size_type SS, std::convertible_to<value_type>... Args>
    constexpr
    StaticArray(const StaticArray<value_type, SS>& subArr, const value_type& v, const Args& ...args) noexcept
    requires (SS + 1 + sizeof...(args) == S) :
        base {S, S, new value_type[S] {}}
    {
        std::copy(subArr.begin(), subArr.end(), base::begin());
        std::initializer_list<value_type> list {v, static_cast<value_type>(args)...};
        std::copy(list.begin(), list.end(), base::begin() + subArr.size());
    }

    constexpr
    StaticArray& operator=(StaticArray other)
    {
        swap(*this, other);
        return *this;
    }

    constexpr
    StaticArray& operator=(StaticArray&& other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    virtual
    ~StaticArray() = default;


};


}
