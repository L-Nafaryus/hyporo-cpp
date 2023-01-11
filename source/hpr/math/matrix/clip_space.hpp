#pragma once

#include "../matrix.hpp"


namespace hpr
{

template <IsReal T>
inline
Matrix<T, 4, 4> ortho(T left, T right, T bottom, T top)
{
    Matrix<T, 4, 4> ms;
    ms.fill(1);
    ms(0, 0) = 2 / (right - left);
    ms(1, 1) = 2 / (top - bottom);
    ms(2, 2) = -1;
    ms(3, 0) = -(right + left) / (right - left);
    ms(3, 1) = -(top + bottom) / (top - bottom);
    return ms;
}

template <IsReal T>
inline
Matrix<T, 4, 4> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    Matrix<T, 4, 4> ms {1};
    //ms.fill(1);
    ms(0, 0) = 2 / (right - left);
    ms(1, 1) = 2 / (top - bottom);
    ms(2, 2) = 2 / (zFar - zNear);
    ms(3, 0) = -(right + left) / (right - left);
    ms(3, 1) = -(top + bottom) / (top - bottom);
    ms(3, 2) = -(zFar + zNear) / (zFar - zNear);
    return ms;
}

template <IsReal T>
inline
Matrix<T, 4, 4> perspective(T fovy, T aspect, T zNear, T zFar)
{
    assert(abs(aspect - std::numeric_limits<T>::epsilon()) > 0);
    Matrix<T, 4, 4> ms;
    const T halfFovyTan = tan(fovy / 2);
    ms(0, 0) = 1 / (aspect * halfFovyTan);
    ms(1, 1) = 1 / halfFovyTan;
    ms(2, 2) = (zFar + zNear) / (zFar - zNear);
    ms(2, 3) = 1;
    ms(3, 2) = -(2 * zFar * zNear) / (zFar - zNear);
    return ms;
}


}
