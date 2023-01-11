#pragma once

#include "../matrix.hpp"


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
Vector<T, 3> translate(const Vector<T, 3>& vs1, const Vector<T, 3>& vs2)
{
    Matrix<T, 4, 4> res = Matrix<T, 4, 4>::identity();
    res.row(3, Vector<T, 4>(vs1, 0.));
    res = translate(res, vs2);

    return Vector<T, 3>(res.row(3));
}

template <IsReal T>
inline
Matrix<T, 4, 4> rotate(const Matrix<T, 4, 4>& ms, const Vector<T, 3>& vs, T angle)
{
    const T cosv = cos(angle);
    const T sinv = sin(angle);
    Vector<T, 3> axis {normalize(vs)};
    Vector<T, 3> temp {(1. - cosv) * axis};

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
Vector<T, 3> rotate(const Vector<T, 3>& vs1, const Vector<T, 3>& vs2, T angle)
{
    Matrix<T, 4, 4> res = Matrix<T, 4, 4>::identity();
    res.row(3, Vector<T, 4>(vs1, 0.));
    res = rotate(res, vs2, angle);

    return Vector<T, 3>(res.row(3));
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
Matrix<T, 4, 4> lookAt(const Matrix<T, 4, 4>& ms, const Vector<T, 3>& eye, const Vector<T, 3>& center, const Vector<T, 3>& up)
{
    const Vector<T, 3> forward {normalize(center - eye)};
    const Vector<T, 3> left {normalize(cross(up, forward))};
    const Vector<T, 3> nup {cross(forward, left)};

    Matrix<T, 4, 4> res;
    res.col(0, Vector<T, 4>(left, 0));
    res.col(1, Vector<T, 4>(nup, 0));
    res.col(2, Vector<T, 4>(forward, 0));
    res.row(3, -Vector<T, 4>(dot(left, eye), dot(nup, eye), dot(forward, eye), -1.));

    return res;
}

}
