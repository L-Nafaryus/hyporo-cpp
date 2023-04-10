#pragma once

#include <hpr/containers/array/sequence.hpp>
#include <hpr/containers/array/dynamic_array.hpp>
#include <numeric>


namespace hpr
{

template <typename T>
constexpr
T sum(const Sequence<T>& s)
{
    return std::accumulate(s.begin(), s.end(), T());
}

template <typename T>
constexpr
T prod(const Sequence<T>& s)
{
    return std::accumulate(s.begin(), s.end(), T(), std::multiplies<T>());
}

template <typename T>
constexpr
bool contain(const Sequence<T>& s, const T& val)
{
    for (const T& el : s)
        if (el == val)
            return true;
    return false;
}

template <typename T>
constexpr
Sequence<T> unique(const Sequence<T>& s)
{
    darray<T> seq;
    for (const T& el : s)
        if (!contain(seq, el))
            seq.push(el);
    return seq;
}

}