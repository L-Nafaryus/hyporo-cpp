#pragma once

#include <hpr/math/matrix/matrix.hpp>


namespace hpr
{

template <IsReal T>
inline
Matrix<T, 4, 4> ortho(T left, T right, T bottom, T top)
{
    Matrix<T, 4, 4> ms = Matrix<T, 4, 4>::identity();
    ms(0, 0) = static_cast<T>(2) / (right - left);
    ms(1, 1) = static_cast<T>(2) / (top - bottom);
    ms(2, 2) = -static_cast<T>(1);
    ms(3, 0) = -(right + left) / (right - left);
    ms(3, 1) = -(top + bottom) / (top - bottom);
    return ms;
}

template <IsReal T>
inline
Matrix<T, 4, 4> ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    Matrix<T, 4, 4> ms = Matrix<T, 4, 4>::identity();
    ms(0, 0) = static_cast<T>(2) / (right - left);
    ms(1, 1) = static_cast<T>(2) / (top - bottom);
    ms(2, 2) = -static_cast<T>(2) / (zFar - zNear);
    ms(0, 3) = -(right + left) / (right - left);
    ms(1, 3) = -(top + bottom) / (top - bottom);
    ms(2, 3) = -(zFar + zNear) / (zFar - zNear);
    return ms;
}

template <IsReal T>
inline
Matrix<T, 4, 4> perspective(T fovy, T aspect, T zNear, T zFar)
{
    assert(abs(aspect - std::numeric_limits<T>::epsilon()) > 0);
    Matrix<T, 4, 4> ms;
    const T halfFovyTan = tan(fovy / 2);
    ms(0, 0) = static_cast<T>(1) / (aspect * halfFovyTan);
    ms(1, 1) = static_cast<T>(1) / halfFovyTan;
    ms(2, 2) = -(zFar + zNear) / (zFar - zNear);
    ms(3, 2) = -static_cast<T>(1);
    ms(2, 3) = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
    return ms;
}


}
