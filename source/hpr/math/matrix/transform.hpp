#pragma once

#include <hpr/math/matrix/matrix.hpp>
#include <hpr/math/vector/vector.hpp>



namespace hpr
{

template <IsReal T>
inline
Matrix<T, 4, 4> translate(const Matrix<T, 4, 4>& ms, const Vector<T, 3>& vs)
{
    Matrix<T, 4, 4> res {ms};
    res.col(3, ms.row(0) * vs[0] + ms.row(1) * vs[1] + ms.row(2) * vs[2] + ms.row(3));

    return res;
}

template <IsReal T>
inline
Matrix<T, 4, 4> rotate(const Matrix<T, 4, 4>& ms, const Vector<T, 3>& vs, T angle)
{
    const T cosv = cos(angle);
    const T sinv = sin(angle);
    Vector<T, 3> axis {normalize(vs)};
    Vector<T, 3> temp {(static_cast<T>(1) - cosv) * axis};

    Matrix<T, 4, 4> rot;
    rot(0, 0) = cosv + temp[0] * axis[0];
    rot(0, 1) = temp[0] * axis[1] + sinv * axis[2];
    rot(0, 2) = temp[0] * axis[2] - sinv * axis[1];
    rot(1, 0) = temp[1] * axis[0] - sinv * axis[2];
    rot(1, 1) = cosv + temp[1] * axis[1];
    rot(1, 2) = temp[1] * axis[2] + sinv * axis[0];
    rot(2, 0) = temp[2] * axis[0] + sinv * axis[1];
    rot(2, 1) = temp[2] * axis[1] - sinv * axis[0];
    rot(2, 2) = cosv + temp[2] * axis[2];

    Matrix<T, 4, 4> res {ms};
    res.row(0, ms.row(0) * rot(0, 0) + ms.row(1) * rot(0, 1) + ms.row(2) * rot(0, 2));
    res.row(1, ms.row(0) * rot(1, 0) + ms.row(1) * rot(1, 1) + ms.row(2) * rot(1, 2));
    res.row(2, ms.row(0) * rot(2, 0) + ms.row(1) * rot(2, 1) + ms.row(2) * rot(2, 2));
    res.row(3, ms.row(3));

    return res;
}


template <IsReal T>
inline
Matrix<T, 4, 4> rotate(const Matrix<T, 4, 4>& ms, const Quaternion& q)
{
    return ms * Matrix<T, 4, 4>(q);
}

template <IsReal T>
inline
Matrix<T, 4, 4> scale(const Matrix<T, 4, 4>& ms, const Vector<T, 3>& vs)
{
    Matrix<T, 4, 4> res;
    res.row(0, ms.row(0) * vs[0]);
    res.row(1, ms.row(1) * vs[1]);
    res.row(2, ms.row(2) * vs[2]);
    res.row(3, ms.row(3));

    return res;
}


template <typename T>
inline
Matrix<T, 4, 4> lookAt(const Vector<T, 3>& eye, const Vector<T, 3>& center, const Vector<T, 3>& up)
{
    const Vector<T, 3> forward {normalize(center - eye)};
    const Vector<T, 3> right {normalize(cross(forward, up))};
    const Vector<T, 3> nup {cross(right, forward)};
    const Vector<T, 3> translation {dot(right, eye), dot(nup, eye), -dot(forward, eye)};

    Matrix<T, 4, 4> res = Matrix<T, 4, 4>::identity();
    res.row(0, Vector<T, 4>(right, 0));
    res.row(1, Vector<T, 4>(nup, 0));
    res.row(2, Vector<T, 4>(-forward, 0));
    res.col(3, Vector<T, 4>(-translation, static_cast<T>(1)));

    return res;
}

}
