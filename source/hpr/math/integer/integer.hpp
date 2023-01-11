#pragma once

#include <type_traits>


namespace hpr
{
    // type traits

    template <typename Type>
    struct is_integer : public std::is_integral<Type> {};

    // concepts

    template <typename Type>
    concept IsInteger = is_integer<Type>::value;
}