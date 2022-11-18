#pragma once

#include "../scalar/scalar.hpp"
#include "vector_space.hpp"


namespace hpr
{

template <typename Type, size_t Size>
using vec = VectorSpace<Type, Size>;

using vec2 = VectorSpace<scalar, 2>;
using vec3 = VectorSpace<scalar, 3>;
using vec4 = VectorSpace<scalar, 4>;

}
