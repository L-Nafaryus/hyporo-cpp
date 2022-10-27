#pragma once

#include <cmath>
#include <limits>


namespace hpr
{

#if defined(PRECISION_FLOAT)

using scalar = float;

#elif defined(PRECISION_DOUBLE)

using scalar = double;

#elif defined(PRECISION_LONGDOUBLE)

using scalar = long double;

#else

using scalar = float;

#endif

static const scalar small = std::numeric_limits<scalar>::epsilon();
static const scalar great = static_cast<scalar>(1.0) / small;
static const scalar valueSmall = std::numeric_limits<scalar>::min();
static const scalar valueGreat = std::numeric_limits<scalar>::max() * 0.1;
static const scalar NaN = std::numeric_limits<scalar>::signaling_NaN();


//- Return 1 if s is positive or 0 otherwise -1
inline int sign(const scalar s)
{
    return (s >= 0) ? 1: -1;
}

inline scalar mag(const scalar s)
{
    return std::fabs(s);
}

// trigonometric

static const scalar PI = static_cast<scalar>(M_PIl);
template <typename Type>
constexpr
Type radians(Type degrees)
{
    static_assert(std::numeric_limits<Type>::is_iec559);
    return degrees * PI / static_cast<Type>(180);
}

template <typename Type>
constexpr
Type degrees(Type radians)
{
    static_assert(std::numeric_limits<Type>::is_iec559);
    return radians * static_cast<Type>(180) / PI;
}

}
