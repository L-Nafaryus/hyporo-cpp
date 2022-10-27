#pragma once

#include "matrix.hpp"


namespace hpr
{

template <typename Type>
inline
MatrixSpace<Type, 4, 4> ortho(Type left, Type right, Type bottom, Type top)
{
    MatrixSpace<Type, 4, 4> ms;
    ms.fill(1);
    ms(0, 0) = 2 / (right - left);
    ms(1, 1) = 2 / (top - bottom);
    ms(2, 2) = -1;
    ms(3, 0) = -(right + left) / (right - left);
    ms(3, 1) = -(top + bottom) / (top - bottom);
    return ms;
}

template <typename Type>
inline
MatrixSpace<Type, 4, 4> ortho(Type left, Type right, Type bottom, Type top, Type zNear, Type zFar)
{
    MatrixSpace<Type, 4, 4> ms;
    ms.fill(1);
    ms(0, 0) = 2 / (right - left);
    ms(1, 1) = 2 / (top - bottom);
    ms(2, 2) = 2 / (zFar - zNear);
    ms(3, 0) = -(right + left) / (right - left);
    ms(3, 1) = -(top + bottom) / (top - bottom);
    ms(3, 2) = -(zFar + zNear) / (zFar - zNear);
    return ms;
}

template <typename Type>
inline
MatrixSpace<Type, 4, 4> perspective(Type fovy, Type aspect, Type zNear, Type zFar)
{
    assert(abs(aspect - std::numeric_limits<Type>::epsilon()) > 0);
    MatrixSpace<Type, 4, 4> ms;
    const Type halfFovyTan = tan(fovy / 2);
    ms(0, 0) = 1 / (aspect * halfFovyTan);
    ms(1, 1) = 1 / halfFovyTan;
    ms(2, 2) = (zFar + zNear) / (zFar - zNear);
    ms(2, 3) = 1;
    ms(3, 2) = -(2 * zFar * zNear) / (zFar - zNear);
    return ms;
}


}
