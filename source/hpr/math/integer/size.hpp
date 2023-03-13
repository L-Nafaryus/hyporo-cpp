#pragma once

#include <type_traits>

namespace hpr
{

    using Size = std::size_t;

    // type traits

    template <typename Type>
    struct is_size : public std::integral_constant<bool, std::is_integral<Type>::value && std::is_unsigned<Type>::value> {};

    // concepts

    template <typename Type>
    concept IsSize = is_size<Type>::value || std::convertible_to<Type, Size>;


    //using Size = typename IsSize<std::size_t>::type;
}