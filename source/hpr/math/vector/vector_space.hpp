#pragma once

#include "../integer.hpp"
#include "../scalar.hpp"
#include "../../containers/array/static_array.hpp"


namespace hpr
{

// forward declarations

template <IsReal T, Size S> requires (S >= 0)
class Vector;

template <IsReal T, Size S>
using SubVector = typename std::conditional<S >= 2, Vector<T, S - 1>, Vector<T, 1>>::type;

// type traits

template <typename T>
struct is_vector : public std::false_type {};

template <typename T, Size S>
struct is_vector<Vector<T, S>> : public std::true_type {};

// concepts

template <typename T>
concept IsVector = is_vector<T>::value;

}

namespace hpr
{

template <IsReal Type, Size S> requires (S >= 0)
class Vector : public StaticArray<Type, S>
{

    using base = StaticArray<Type, S>;

public:

    using value_type = Type;
    using size_type = Size;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator<Type>;
    using const_iterator = Iterator<const Type>;

public:

    //! null constructor
    constexpr
    Vector() :
        base {}
    {}

    //! copy constructor
    constexpr
    Vector(const Vector& vs) :
        base {static_cast<base>(vs)}
    {}

    //! move constructor
    constexpr
    Vector(Vector&& vs) noexcept :
        base {std::forward<base>(static_cast<base>(vs))}
    {}

    //! copy assignment operator
    constexpr
    Vector& operator=(const Vector& vs)
    {
        base::operator=(vs);
        return *this;
    }

    //! move assignment operator
    constexpr
    Vector& operator=(Vector&& vs) noexcept
    {
        swap(*this, vs);
        return *this;
    }

    //! destructor
    virtual
    ~Vector() = default;

    //! copy constructor from base
    constexpr
    Vector(const base& arr) :
        base {arr}
    {}

    //! move constructor from base
    constexpr
    Vector(base&& arr) :
        base {std::forward<base>(arr)}
    {}

    //! construct from iterators
    constexpr
    Vector(typename base::iterator start, typename base::iterator end) :
        base {start, end}
    {}

    //! construct from constant iterators
    constexpr
    Vector(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end}
    {}

    //! construct from initializer list
    constexpr
    Vector(std::initializer_list<value_type> list) :
        base {list}
    {}

    //! copy constructor with variadic args
    template <IsReal... Args>
    constexpr
    Vector(const value_type& v, const Args& ...args) requires (S == 1 + sizeof...(args)):
        base {v, static_cast<value_type>(args)...}
    {}

    //! move constructor with variadic args
    template <IsReal... Args>
    constexpr
    Vector(value_type&& v, Args&& ...args) requires (S == 1 + sizeof...(args)):
        base {v, static_cast<value_type>(std::forward<Args>(args))...}
    {}

    //! copy constructor with sub vector and value
    constexpr
    Vector(const SubVector<Type, S>& svs, const value_type& v) requires (S >= 1):
        base {}
    {
        for (auto n = 0; n < svs.size(); ++n)
            (*this)[n] = svs[n];
        (*this)[svs.size()] = v;
    }

    //! copy constructor from greater vector
    template <Size GS> requires (GS > S)
    constexpr explicit
    Vector(const Vector<Type, GS>& vs) :
        base {vs.begin(), vs.begin() + S}
    {}

};

// global operators

template <IsReal T, Size S> inline Vector<T, S> operator+(const Vector<T, S>& lhs) { Vector<T, S> vs; for (Size n = 0; n < S; ++n) vs[n] = lhs[n]; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator-(const Vector<T, S>& lhs) { Vector<T, S> vs; for (Size n = 0; n < S; ++n) vs[n] = -lhs[n]; return vs; }

template <IsReal T, Size S> inline Vector<T, S>& operator+=(Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) lhs[n] += rhs[n]; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator-=(Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) lhs[n] -= rhs[n]; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator*=(Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) lhs[n] *= rhs[n]; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator/=(Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) lhs[n] /= rhs[n]; return lhs; }

template <IsReal T, Size S> inline Vector<T, S> operator+(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] += rhs[n]; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator-(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] -= rhs[n]; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator*(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] *= rhs[n]; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator/(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] /= rhs[n]; return vs; }

template <IsReal T, Size S> inline bool operator==(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) if (lhs[n] != rhs[n]) return false; return true; }
template <IsReal T, Size S> inline bool operator!=(const Vector<T, S>& lhs, const Vector<T, S>& rhs) { for (Size n = 0; n < S; ++n) if (lhs[n] == rhs[n]) return false; return true; }


template <IsReal T, Size S> inline Vector<T, S>& operator+=(Vector<T, S>& lhs, const T& rhs) { for (Size n = 0; n < S; ++n) lhs[n] += rhs; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator-=(Vector<T, S>& lhs, const T& rhs) { for (Size n = 0; n < S; ++n) lhs[n] -= rhs; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator*=(Vector<T, S>& lhs, const T& rhs) { for (Size n = 0; n < S; ++n) lhs[n] *= rhs; return lhs; }
template <IsReal T, Size S> inline Vector<T, S>& operator/=(Vector<T, S>& lhs, const T& rhs) { for (Size n = 0; n < S; ++n) lhs[n] /= rhs; return lhs; }

template <IsReal T, Size S> inline Vector<T, S> operator+(const Vector<T, S>& lhs, const T& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] += rhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator-(const Vector<T, S>& lhs, const T& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] -= rhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator*(const Vector<T, S>& lhs, const T& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] *= rhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator/(const Vector<T, S>& lhs, const T& rhs) { Vector<T, S> vs {lhs}; for (Size n = 0; n < S; ++n) vs[n] /= rhs; return vs; }

template <IsReal T, Size S> inline Vector<T, S> operator+(const T& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {rhs}; for (Size n = 0; n < S; ++n) vs[n] += lhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator-(const T& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {rhs}; for (Size n = 0; n < S; ++n) vs[n] -= lhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator*(const T& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {rhs}; for (Size n = 0; n < S; ++n) vs[n] *= lhs; return vs; }
template <IsReal T, Size S> inline Vector<T, S> operator/(const T& lhs, const Vector<T, S>& rhs) { Vector<T, S> vs {rhs}; for (Size n = 0; n < S; ++n) vs[n] /= lhs; return vs; }

// boolean operations

template <typename Type, Size S>
inline
Vector<bool, S> equal(const Vector<Type, S>& lhs, const Vector<Type, S>& rhs, const scalar& precision = scalar::precision())
{
    Vector<bool, S> vs;
    for (auto n = 0; n < S; ++n)
        vs[n] = equal(lhs[n], rhs[n], precision);
    return vs;
}

template <Size S>
inline
bool any(const Vector<bool, S>& vs)
{
    bool res = false;
    for (auto e : vs)
        res = res || e;
    return res;
}

template <Size S>
inline
bool all(const Vector<bool, S>& vs)
{
    bool res = true;
    for (auto e : vs)
        res = res && e;
    return res;
}

// per element operations

template <typename Type, Size S>
inline
Vector<Type, S> abs(const Vector<Type, S>& vs)
{
    Vector<Type, S> res;
    for (auto n = 0; n < S; ++n)
        res[n] = abs(vs[n]);
    return res;
}

template <typename Type, Size S>
inline
Type sum(const Vector<Type, S>& vs)
{
    Type sum {};
    for (const Type& v : vs)
        sum += v;
    return sum;
}

template <typename Type, Size S>
inline
Vector<Type, S> pow(const Vector<Type, S>& vs, scalar degree)
{
    Vector<Type, S> res;
    for (auto n = 0; n < S; ++n)
        res[n] = pow(vs[n], degree);
    return res;
}

// vector operations

template <typename Type, Size S>
inline
Type norm(const Vector<Type, S>& vs)
{
    return sqrt(sum(pow(abs(vs), 2)));
}

template <typename Type, Size S>
inline
Type dot(const Vector<Type, S>& lhs, const Vector<Type, S>& rhs)
{
    return sum(lhs * rhs);
}

template <typename Type, Size S>
inline
Type length(const Vector<Type, S>& vs)
{
    return sqrt(dot(vs, vs));
}

template <typename Type, Size S>
inline
Type mag(const Vector<Type, S>& vs)
{
    return length(vs);
}

template <typename Type, Size S>
inline
Type distance(const Vector<Type, S>& vs1, const Vector<Type, S>& vs2)
{
    return length(vs1 - vs2);
}

template <typename Type, Size S>
inline
Vector<Type, S> normalize(const Vector<Type, S>& vs)
{
    return vs * isqrt(dot(vs, vs));
}

template <typename Type>
inline
Type angle(const Vector<Type, 3>& lhs, const Vector<Type, 3>& rhs)
{
    scalar cos = dot(lhs, rhs) / (norm(lhs) * norm(rhs));
    return acos(cos); //clip(cos, -1., 1.));
}

// vector 3 operations

template <typename Type>
inline
Vector<Type, 3> cross(const Vector<Type, 3>& lhs, const Vector<Type, 3>& rhs)
{
    return Vector<Type, 3>(
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    );
}


// aliases

template <typename Type, Size S>
using vec = Vector<Type, S>;

using vec2 = Vector<scalar, 2>;
using vec3 = Vector<scalar, 3>;
using vec4 = Vector<scalar, 4>;

}
