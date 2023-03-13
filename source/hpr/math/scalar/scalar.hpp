#pragma once

#include <cmath>
#include <limits>
#include <hpr/math/integer.hpp>


namespace hpr
{

// type traits

template <typename Type>
struct is_scalar : public std::is_floating_point<Type> {};

// concepts

template <typename Type>
concept IsScalar = is_scalar<Type>::value;

template <typename Type>
concept IsReal = is_integer<Type>::value || is_scalar<Type>::value;

// forward declaration

template <IsScalar T>
class Scalar;

}

namespace std
{
    template <class T> struct is_floating_point<hpr::Scalar<T>> : true_type {};
}

namespace hpr
{

// class declaration

template <IsScalar T>
class Scalar
{

public:

    using type = Scalar<T>;
    using value_type = T;

protected:

    value_type p_value;

public:

    // constructors

    constexpr Scalar() : p_value {} {}

    template <IsScalar X> constexpr Scalar(const Scalar<X>& value) : p_value {static_cast<value_type>(value.p_value)} {}

    template <IsReal X> constexpr Scalar(const X& value) : p_value {static_cast<value_type>(value)} {}

    template <IsScalar X> constexpr type& operator=(const Scalar<X>& value) { p_value = static_cast<value_type>(value.p_value); return *this; }

    template <IsReal X> constexpr type& operator=(const X& value) { p_value = static_cast<value_type>(value); return *this; }

    virtual constexpr ~Scalar() = default;

    // conversion

    constexpr operator double() const { return static_cast<double>(p_value); }

    constexpr operator float() const { return static_cast<float>(p_value); }

    constexpr operator long double() const { return static_cast<long double>(p_value); }

    constexpr operator bool() const { return static_cast<bool>(p_value); }

    // access

    [[nodiscard]] constexpr value_type value() const { return p_value; }

    constexpr value_type& value() { return p_value; }

    // properties

/*protected:

    static value_type s_precision;

public:

    static constexpr Scalar<T> precision() { return s_precision; }

    static constexpr void precision(const value_type& precision) { s_precision = precision; }

    static constexpr Scalar<T> inf() { return std::numeric_limits<value_type>::infinity(); }

    static constexpr Scalar<T> epsilon() { return std::numeric_limits<value_type>::epsilon(); }*/
};

// specialization type

#if defined(HPR_SCALAR_IMPLEMENTAION)
#if defined(HPR_SCALAR_LONGDOUBLE)
    using scalar_type = long double;
    using scalar = Scalar<long double>;
#elif defined(HPR_SCALAR_DOUBLE)
    using scalar_type = double;
    using scalar = Scalar<double>;
#elif defined(HPR_SCALAR_FLOAT)
    using scalar_type = float;
    using scalar = Scalar<float>;
#else
    using scalar_type = double;
    using scalar = Scalar<double>;
#endif
#else
#if defined(HPR_SCALAR_LONGDOUBLE)
    using scalar_type = long double;
    using scalar = long double;
#elif defined(HPR_SCALAR_DOUBLE)
    using scalar_type = double;
    using scalar = double;
#elif defined(HPR_SCALAR_FLOAT)
    using scalar_type = float;
    using scalar = float;
#else
    using scalar_type = float;
    using scalar = float;
#endif
#endif

//

#if defined(HPR_SCALAR_IMPLEMENTATION)
template<> scalar::value_type scalar::s_precision = static_cast<scalar::value_type>(1e-15);

// global operators

/// scalar vs scalar

constexpr scalar operator+(const scalar& s) { return s; }
constexpr scalar operator-(const scalar& s) { return -s.value(); }
constexpr bool operator!(const scalar& s) { return !static_cast<bool>(s.value()); }

constexpr scalar& operator+=(scalar& lhs, const scalar& rhs) { lhs.value() += static_cast<scalar::value_type>(rhs.value()); return lhs; }
constexpr scalar& operator-=(scalar& lhs, const scalar& rhs) { lhs.value() -= static_cast<scalar::value_type>(rhs.value()); return lhs; }
constexpr scalar& operator*=(scalar& lhs, const scalar& rhs) { lhs.value() *= static_cast<scalar::value_type>(rhs.value()); return lhs; }
constexpr scalar& operator/=(scalar& lhs, const scalar& rhs) { lhs.value() /= static_cast<scalar::value_type>(rhs.value()); return lhs; }

constexpr scalar operator+(const scalar& lhs, const scalar& rhs) { return lhs.value() + rhs.value(); }
constexpr scalar operator-(const scalar& lhs, const scalar& rhs) { return lhs.value() - rhs.value(); }
constexpr scalar operator*(const scalar& lhs, const scalar& rhs) { return lhs.value() * rhs.value(); }
constexpr scalar operator/(const scalar& lhs, const scalar& rhs) { return lhs.value() / rhs.value(); }

constexpr bool operator==(const scalar& lhs, const scalar& rhs) { return lhs.value() == rhs.value(); }
constexpr bool operator!=(const scalar& lhs, const scalar& rhs) { return lhs.value() != rhs.value(); }
constexpr bool operator&&(const scalar& lhs, const scalar& rhs) { return static_cast<bool>(lhs) && static_cast<bool>(rhs); }
constexpr bool operator||(const scalar& lhs, const scalar& rhs) { return static_cast<bool>(lhs) || static_cast<bool>(rhs); }
constexpr bool operator>(const scalar& lhs, const scalar& rhs) { return lhs.value() > rhs.value(); }
constexpr bool operator<(const scalar& lhs, const scalar& rhs) { return lhs.value() < rhs.value(); }
constexpr bool operator>=(const scalar& lhs, const scalar& rhs) { return lhs.value() >= rhs.value(); }
constexpr bool operator<=(const scalar& lhs, const scalar& rhs) { return lhs.value() <= rhs.value(); }

//std::ostream& operator<<(std::ostream& stream, const scalar& s) { return stream << s.value(); }

/// scalar vs Scalar<T>

template <IsScalar T> constexpr scalar& operator+=(scalar& lhs, const Scalar<T>& rhs) { lhs.value() += static_cast<scalar::value_type>(rhs.value()); return lhs; }
template <IsScalar T> constexpr scalar& operator-=(scalar& lhs, const Scalar<T>& rhs) { lhs.value() -= static_cast<scalar::value_type>(rhs.value()); return lhs; }
template <IsScalar T> constexpr scalar& operator*=(scalar& lhs, const Scalar<T>& rhs) { lhs.value() *= static_cast<scalar::value_type>(rhs.value()); return lhs; }
template <IsScalar T> constexpr scalar& operator/=(scalar& lhs, const Scalar<T>& rhs) { lhs.value() /= static_cast<scalar::value_type>(rhs.value()); return lhs; }

template <IsScalar T> constexpr scalar operator+(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() + static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr scalar operator+(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) + rhs.value(); }
template <IsScalar T> constexpr scalar operator-(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() - static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr scalar operator-(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) - rhs.value(); }
template <IsScalar T> constexpr scalar operator*(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() * static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr scalar operator*(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) * rhs.value(); }
template <IsScalar T> constexpr scalar operator/(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() / static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr scalar operator/(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) / rhs.value(); }

template <IsScalar T> constexpr bool operator==(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() == static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator==(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) == rhs.value(); }
template <IsScalar T> constexpr bool operator!=(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() != static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator!=(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) != rhs.value(); }
template <IsScalar T> constexpr bool operator&&(const scalar& lhs, const Scalar<T>& rhs) { return static_cast<bool>(lhs) && static_cast<bool>(rhs); }
template <IsScalar T> constexpr bool operator&&(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<bool>(lhs) && static_cast<bool>(rhs); }
template <IsScalar T> constexpr bool operator||(const scalar& lhs, const Scalar<T>& rhs) { return static_cast<bool>(lhs) || static_cast<bool>(rhs); }
template <IsScalar T> constexpr bool operator||(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<bool>(lhs) || static_cast<bool>(rhs); }
template <IsScalar T> constexpr bool operator>(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() > static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator>(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) > rhs.value(); }
template <IsScalar T> constexpr bool operator<(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() < static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator<(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) < rhs.value(); }
template <IsScalar T> constexpr bool operator>=(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() >= static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator>=(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) >= rhs.value(); }
template <IsScalar T> constexpr bool operator<=(const scalar& lhs, const Scalar<T>& rhs) { return lhs.value() <= static_cast<scalar::value_type>(rhs.value()); }
template <IsScalar T> constexpr bool operator<=(const Scalar<T>& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs.value()) <= rhs.value(); }

template <IsScalar T> std::ostream& operator<<(std::ostream& stream, const Scalar<T>& s) { return stream << s.value(); }
template <IsScalar T> std::istream& operator>>(std::istream& stream, Scalar<T>& s) { return stream >> s.value(); }

/// scalar vs real

template <IsReal T> constexpr scalar& operator+=(scalar& lhs, const T& rhs) { lhs.value() += static_cast<scalar::value_type>(rhs); return lhs; }
template <IsReal T> constexpr scalar& operator-=(scalar& lhs, const T& rhs) { lhs.value() -= static_cast<scalar::value_type>(rhs); return lhs; }
template <IsReal T> constexpr scalar& operator*=(scalar& lhs, const T& rhs) { lhs.value() *= static_cast<scalar::value_type>(rhs); return lhs; }
template <IsReal T> constexpr scalar& operator/=(scalar& lhs, const T& rhs) { lhs.value() /= static_cast<scalar::value_type>(rhs); return lhs; }

template <IsReal T> constexpr T& operator+=(T& lhs, const scalar& rhs) { lhs += static_cast<T>(rhs); return lhs; }
template <IsReal T> constexpr T& operator-=(T& lhs, const scalar& rhs) { lhs -= static_cast<T>(rhs); return lhs; }
template <IsReal T> constexpr T& operator*=(T& lhs, const scalar& rhs) { lhs *= static_cast<T>(rhs); return lhs; }
template <IsReal T> constexpr T& operator/=(T& lhs, const scalar& rhs) { lhs /= static_cast<T>(rhs); return lhs; }

template <IsReal T> constexpr scalar operator+(const scalar& lhs, const T& rhs) { return lhs.value() + static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr scalar operator+(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) + rhs.value(); }
template <IsReal T> constexpr scalar operator-(const scalar& lhs, const T& rhs) { return lhs.value() - static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr scalar operator-(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) - rhs.value(); }
template <IsReal T> constexpr scalar operator*(const scalar& lhs, const T& rhs) { return lhs.value() * static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr scalar operator*(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) * rhs.value(); }
template <IsReal T> constexpr scalar operator/(const scalar& lhs, const T& rhs) { return lhs.value() / static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr scalar operator/(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) / rhs.value(); }

template <IsReal T> constexpr bool operator==(const scalar& lhs, const T& rhs) { return lhs.value() == static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator==(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) == rhs.value(); }
template <IsReal T> constexpr bool operator!=(const scalar& lhs, const T& rhs) { return lhs.value() != static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator!=(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) != rhs.value(); }
template <IsReal T> constexpr bool operator&&(const scalar& lhs, const T& rhs) { return static_cast<bool>(lhs) && static_cast<bool>(rhs); }
template <IsReal T> constexpr bool operator&&(const T& lhs, const scalar& rhs) { return static_cast<bool>(lhs) && static_cast<bool>(rhs); }
template <IsReal T> constexpr bool operator||(const scalar& lhs, const T& rhs) { return static_cast<bool>(lhs) || static_cast<bool>(rhs); }
template <IsReal T> constexpr bool operator||(const T& lhs, const scalar& rhs) { return static_cast<bool>(lhs) || static_cast<bool>(rhs); }
template <IsReal T> constexpr bool operator>(const scalar& lhs, const T& rhs) { return lhs.value() > static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator>(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) > rhs.value(); }
template <IsReal T> constexpr bool operator<(const scalar& lhs, const T& rhs) { return lhs.value() < static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator<(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) < rhs.value(); }
template <IsReal T> constexpr bool operator>=(const scalar& lhs, const T& rhs) { return lhs.value() >= static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator>=(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) >= rhs.value(); }
template <IsReal T> constexpr bool operator<=(const scalar& lhs, const T& rhs) { return lhs.value() <= static_cast<scalar::value_type>(rhs); }
template <IsReal T> constexpr bool operator<=(const T& lhs, const scalar& rhs) { return static_cast<scalar::value_type>(lhs) <= rhs.value(); }

#endif

// transcendentals

template <IsReal T> constexpr scalar cos(const T& s) { return std::cos(static_cast<scalar_type>(s));}

template <IsReal T> constexpr scalar acos(const T& s) { return std::acos(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar cosh(const T& s) { return std::cosh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar acosh(const T& s) { return std::acosh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar sin(const T& s) { return std::sin(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar asin(const T& s) { return std::asin(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar sinh(const T& s) { return std::sinh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar asinh(const T& s) { return std::asinh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar tan(const T& s) { return std::tan(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar atan(const T& s) { return std::atan(static_cast<scalar_type>(s)); }

template <IsReal T, IsReal X> constexpr scalar atan2(const T& s, const X& s2) { return std::atan2(static_cast<scalar_type>(s), static_cast<scalar_type>(s2)); }

template <IsReal T> constexpr scalar tanh(const T& s) { return std::tanh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar atanh(const T& s) { return std::atanh(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar exp(const T& s) { return std::exp(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar log(const T& s) { return std::log(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar log10(const T& s) { return std::log10(static_cast<scalar_type>(s)); }

template <IsReal T, IsReal X> constexpr scalar pow(const T& s, const X& d) { return std::pow(static_cast<scalar_type>(s), static_cast<scalar_type>(d)); }

template <IsReal T> constexpr scalar sqrt(const T& s) { return std::sqrt(static_cast<scalar_type>(s)); }

template <IsReal T> constexpr scalar isqrt(const T& s) { return static_cast<T>(1) / sqrt(static_cast<scalar_type>(s)); }

// constants

constexpr scalar pi() { return std::numbers::pi_v<scalar_type>; }

constexpr scalar e() { return std::numbers::e_v<scalar_type>; }

// etc

static float _scalar_float_precision = 1e-15f;
static double _scalar_double_precision = 1e-15;
static long double _scalar_long_double_precision = 1e-15l;

template <IsReal T> inline void precision(const T& precision)
{
    if constexpr (std::is_same<T, float>::value)
        _scalar_float_precision = precision;
    else if constexpr (std::is_same<T, double>::value)
        _scalar_double_precision = precision;
    else if constexpr (std::is_same<T, long double>::value)
        _scalar_long_double_precision = precision;
}

template <IsReal T> inline T precision()
{
    if constexpr (std::is_same<T, float>::value)
        return _scalar_float_precision;
    else if constexpr (std::is_same<T, double>::value)
        return _scalar_double_precision;
    else if constexpr (std::is_same<T, long double>::value)
        return _scalar_long_double_precision;
}



constexpr scalar inf() { return std::numeric_limits<scalar_type>::infinity(); }

constexpr scalar epsilon() { return std::numeric_limits<scalar_type>::epsilon(); }

constexpr scalar isnan(const scalar& s) { return std::isnan(static_cast<scalar_type>(s)); }

constexpr scalar abs(const scalar& s) { return std::abs(static_cast<scalar_type>(s)); }

constexpr scalar mag(const scalar& s) { return std::abs(static_cast<scalar_type>(s)); }

constexpr bool equal(const scalar& lhs, const scalar& rhs, const scalar& precision = hpr::precision<scalar>()) { return abs(lhs - rhs) < precision; }

//! Convert degrees to radians
constexpr scalar rad(const scalar& s) { return s * pi() / static_cast<scalar_type>(180); }

//! Convert radians to degrees
constexpr scalar deg(const scalar& s) { return s * static_cast<scalar_type>(180) / pi(); }

constexpr scalar min(const scalar& s1, const scalar& s2) { return std::min(static_cast<scalar_type>(s1), static_cast<scalar_type>(s2));}

constexpr scalar max(const scalar& s1, const scalar& s2) { return std::max(static_cast<scalar_type>(s1), static_cast<scalar_type>(s2)); }

constexpr scalar clip(const scalar& s, const scalar& sMin, const scalar& sMax) { return min(sMax, max(s, sMin)); }

}

/*namespace std
{
    // compatibility with std

    template <hpr::IsScalar T> constexpr hpr::Scalar<T> cos(const hpr::Scalar<T>& s) { return hpr::cos(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> acos(const hpr::Scalar<T>& s) { return hpr::acos(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> cosh(const hpr::Scalar<T>& s) { return hpr::cosh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> acosh(const hpr::Scalar<T>& s) { return hpr::acosh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> sin(const hpr::Scalar<T>& s) { return hpr::sin(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> asin(const hpr::Scalar<T>& s) { return hpr::asin(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> sinh(const hpr::Scalar<T>& s) { return hpr::sinh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> asinh(const hpr::Scalar<T>& s) { return hpr::asinh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> tan(const hpr::Scalar<T>& s) { return hpr::tan(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> atan(const hpr::Scalar<T>& s) { return hpr::atan(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> tanh(const hpr::Scalar<T>& s) { return hpr::tanh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> atanh(const hpr::Scalar<T>& s) { return hpr::atanh(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> exp(const hpr::Scalar<T>& s) { return hpr::exp(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> log(const hpr::Scalar<T>& s) { return hpr::log(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> log10(const hpr::Scalar<T>& s) { return hpr::log10(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> sqrt(const hpr::Scalar<T>& s) { return hpr::sqrt(s); }
    template <hpr::IsScalar T> constexpr hpr::Scalar<T> abs(const hpr::Scalar<T>& s) { return hpr::abs(s); }
    template <hpr::IsScalar T, hpr::IsScalar X> constexpr hpr::Scalar<T> pow(const hpr::Scalar<T>& s, const hpr::Scalar<X>& d) { return hpr::pow(s, d); }
    template <hpr::IsScalar T, hpr::IsReal X> constexpr hpr::Scalar<T> pow(const hpr::Scalar<T>& s, const X& d) { return hpr::pow(s, d); }
    template <hpr::IsReal T, hpr::IsScalar X> constexpr hpr::Scalar<T> pow(const T& s, const hpr::Scalar<X>& d) { return hpr::pow(s, d); }
}*/
