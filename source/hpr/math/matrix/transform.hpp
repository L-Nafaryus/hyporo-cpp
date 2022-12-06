#pragma once

#include "../matrix.hpp"


namespace hpr
{

template <typename Type>
inline
mat<Type, 4, 4> translate(const mat<Type, 4, 4>& ms, const vec<Type, 3>& vs)
{
    mat<Type, 4, 4> res {ms};
    res.col(3, ms.row(0) * vs[0] + ms.row(1) * vs[1] + ms.row(2) * vs[2] + ms.row(3));

    return res;
}

template <typename Type>
inline
vec<Type, 3> translate(const vec<Type, 3>& vs1, const vec<Type, 3>& vs2)
{
    mat<Type, 4, 4> res = mat<Type, 4, 4>::identity();
    res.row(3, vec<Type, 4>(vs1, 0.));
    res = translate(res, vs2);

    return vec<Type, 3>(res.row(3));
}

template <typename Type>
inline
mat<Type, 4, 4> rotate(const mat<Type, 4, 4>& ms, const vec<Type, 3>& vs, Type angle)
{
    const Type cosv = cos(angle);
    const Type sinv = sin(angle);
    vec<Type, 3> axis {normalize(vs)};
    vec<Type, 3> temp {(1. - cosv) * axis};

    mat<Type, 4, 4> rot;
    rot(0, 0) = cosv + temp[0] * axis[0];
    rot(0, 1) = temp[0] * axis[1] + sinv * axis[2];
    rot(0, 2) = temp[0] * axis[2] - sinv * axis[1];
    rot(1, 0) = temp[1] * axis[0] - sinv * axis[2];
    rot(1, 1) = cosv + temp[1] * axis[1];
    rot(1, 2) = temp[1] * axis[2] + sinv * axis[0];
    rot(2, 0) = temp[2] * axis[0] + sinv * axis[1];
    rot(2, 1) = temp[2] * axis[1] - sinv * axis[0];
    rot(2, 2) = cosv + temp[2] * axis[2];

    mat<Type, 4, 4> res {ms};
    res.row(0, ms.row(0) * rot(0, 0) + ms.row(1) * rot(0, 1) + ms.row(2) * rot(0, 2));
    res.row(1, ms.row(0) * rot(1, 0) + ms.row(1) * rot(1, 1) + ms.row(2) * rot(1, 2));
    res.row(2, ms.row(0) * rot(2, 0) + ms.row(1) * rot(2, 1) + ms.row(2) * rot(2, 2));
    res.row(3, ms.row(3));

    return res;
}

template <typename Type>
inline
vec<Type, 3> rotate(const vec<Type, 3>& vs1, const vec<Type, 3>& vs2, Type angle)
{
    mat<Type, 4, 4> res = mat<Type, 4, 4>::identity();
    res.row(3, vec<Type, 4>(vs1, 0.));
    res = rotate(res, vs2, angle);

    return vec<Type, 3>(res.row(3));
}

template <typename Type>
inline
mat<Type, 4, 4> scale(const mat<Type, 4, 4>& ms, const vec<Type, 3>& vs)
{
    mat<Type, 4, 4> res;
    res.row(0, ms.row(0) * vs[0]);
    res.row(1, ms.row(1) * vs[1]);
    res.row(2, ms.row(2) * vs[2]);
    res.row(3, ms.row(3));

    return res;
}


template <typename Type>
inline
mat<Type, 4, 4> lookAt(const mat<Type, 4, 4>& ms, const vec<Type, 3>& eye, const vec<Type, 3>& center, const vec<Type, 3>& up)
{
    const vec<Type, 3> forward {normalize(center - eye)};
    const vec<Type, 3> left {normalize(cross(up, forward))};
    const vec<Type, 3> nup {cross(forward, left)};

    mat<Type, 4, 4> res;
    res.col(0, vec<Type, 4>(left, 0));
    res.col(1, vec<Type, 4>(nup, 0));
    res.col(2, vec<Type, 4>(forward, 0));
    res.row(3, -vec<Type, 4>(dot(left, eye), dot(nup, eye), dot(forward, eye), -1.));

    return res;
}

}
