#pragma once

#include <cmath>
#include <limits>


namespace hyporo
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
static const scalar great = 1.0 / small;
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


} // end namespace hyporo
