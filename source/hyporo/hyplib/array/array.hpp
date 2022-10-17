#pragma once

#include "dynamic_array.hpp"
#include "static_array.hpp"


namespace hpr
{

template <typename Type>
using darray = DynamicArray<Type>;

template <typename Type, size_t Size>
using sarray = StaticArray<Type, Size>;

}