#pragma once

#include "../array/array.hpp"

namespace hpr
{

template <typename Type, size_t Size>
class VectorSpace : public StaticArray<Type, Size>
{
    static_assert(std::is_arithmetic<Type>::value, "Type must be numeric");

    using base = StaticArray<Type, Size>;

    using SubVector = typename std::conditional<Size >= 2, VectorSpace<Type, Size - 1>, VectorSpace<Type, 1>>::type;
public:

    using value_type = Type;
    using size_type = size_t;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator<Type>;
    using const_iterator = Iterator<const Type>;

public:

    inline
    VectorSpace() :
        base {}
    {}

    inline
    VectorSpace(const VectorSpace& vs) :
        base {static_cast<base>(vs)}
    {}

    inline
    VectorSpace(VectorSpace&& vs) noexcept :
        base {std::move(static_cast<base>(vs))}
    {}

    inline
    VectorSpace(typename base::iterator start, typename base::iterator end) :
        base {start, end}
    {}

    inline
    VectorSpace(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end}
    {}

    inline
    VectorSpace(std::initializer_list<value_type> list) :
        base {list}
    {}

    template <std::convertible_to<value_type>... Args>
    inline
    VectorSpace(value_type&& v, Args&& ...args) :
        base {v, static_cast<value_type>(std::forward<Args>(args))...}
    {
        static_assert(1 + sizeof...(args) == Size, "Number of arguments must be equal to size of vector");
    }

    /*template <size_type SubSize, std::convertible_to<value_type>... Args>
    inline
    VectorSpace(const VectorSpace<value_type, SubSize>& subvec, const value_type& v, const Args& ...args) :
        base {static_cast<StaticArray<value_type, SubSize>>(subvec), v,
              static_cast<value_type>(std::forward<Args>(args))...}
    {}*/

    inline
    VectorSpace(const SubVector& subvs, const value_type& v) :
        base {}
    {
        for (auto n = 0; n < subvs.size(); ++n)
            (*this)[n] = subvs[n];
        (*this)[subvs.size() - 1] = v;
    }

    // Member functions

    // vector versus scalar (per element operations)

    friend inline
    VectorSpace operator-(const VectorSpace& rhs)
    {
        VectorSpace vs {rhs};
        for (value_type& v : vs)
            v = -v;
        return vs;
    }

    inline
    void operator*=(const value_type& val)
    {
        //for (value_type& v : *this)
        //    v *= val;
        for (auto n = 0; n < Size; ++n)
            (*this)[n] *= val;
    }

    inline
    void operator+=(const value_type& val)
    {
        for (auto n = 0; n < Size; ++n)
            (*this)[n] += val;
    }

    inline
    void operator-=(const value_type& val)
    {
        //for (value_type& v : *this)
        //    v -= val;
        for (auto n = 0; n < Size; ++n)
            (*this)[n] -= val;
    }

    inline
    void operator/=(const value_type& val)
    {
        for (value_type& v : *this)
            v /= val;
    }

    using type = VectorSpace<Type, Size>;

    friend inline
    type operator+(const VectorSpace& lhs, const value_type& rhs)
    {
        VectorSpace vs {lhs};
        vs += rhs;
        return vs;
    }

    friend inline
    VectorSpace operator+(const value_type& lhs, const VectorSpace& rhs)
    {
        return operator+(rhs, lhs);
    }

    friend inline
    VectorSpace operator*(const VectorSpace& lhs, const value_type& rhs)
    {
        VectorSpace vs {lhs};
        vs *= rhs;
        return vs;
    }

    friend inline
    VectorSpace operator*(const value_type& lhs, const VectorSpace& rhs)
    {
        return operator*(rhs, lhs);
    }

    friend inline
    VectorSpace operator/(const VectorSpace& lhs, const value_type& rhs)
    {
        VectorSpace vs {lhs};
        vs /= rhs;
        return vs;
    }

    friend inline
    VectorSpace operator/(const value_type& lhs, const VectorSpace& rhs)
    {
        VectorSpace vs;
        for (auto n = 0; n < vs.size(); ++n)
            vs[n] = lhs / rhs[n];
        return vs;
    }

    // vector versus vector (per element operations)

    inline
    void operator*=(const VectorSpace& vs)
    {
        for (auto n = 0; n < Size; ++n)
            (*this)[n] *= vs[n];
    }

    inline
    void operator+=(const VectorSpace& vs)
    {
        for (auto n = 0; n < Size; ++n)
            (*this)[n] += vs[n];
    }

    inline
    void operator-=(const VectorSpace& vs)
    {
        for (auto n = 0; n < Size; ++n)
            (*this)[n] -= vs[n];
    }

    inline
    void operator/=(const VectorSpace& vs)
    {
        for (auto n = 0; n < Size; ++n)
            (*this)[n] /= vs[n];
    }

    friend inline
    VectorSpace operator+(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        VectorSpace vs {lhs};
        vs += rhs;
        return vs;
    }

    friend inline
    VectorSpace operator-(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        VectorSpace vs {lhs};
        vs -= rhs;
        return vs;
    }

    friend inline
    VectorSpace operator*(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        VectorSpace vs {lhs};
        vs *= rhs;
        return vs;
    }

    friend inline
    VectorSpace operator/(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        VectorSpace vs {lhs};
        vs /= rhs;
        return vs;
    }

    friend inline
    bool operator==(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        for (auto n = 0; n < Size; ++n)
            if (lhs[n] != rhs[n])
                return false;
        return true;
    }

    friend inline
    bool operator!=(const VectorSpace& lhs, const VectorSpace& rhs)
    {
        return !(lhs == rhs);
    }


};

template <typename Type, size_t Size>
inline
Type sum(const VectorSpace<Type, Size>& vs)
{
    Type sum {0};
    for (const Type& v : vs)
        sum += v;
    return sum;
}

template <typename Type, size_t Size>
constexpr
Type dot(const VectorSpace<Type, Size>& lhs, const VectorSpace<Type, Size>& rhs)
{
    return sum(lhs * rhs);
}

template <typename Type, size_t Size>
inline
Type length(const VectorSpace<Type, Size>& vs)
{
    return sqrt(dot(vs, vs));
}

template <typename Type, size_t Size>
inline
Type distance(const VectorSpace<Type, Size>& point1, const VectorSpace<Type, Size>& point2)
{
    return length(point1 - point2);
}

template <typename Type>
constexpr
VectorSpace<Type, 3> cross(const VectorSpace<Type, 3>& lhs, const VectorSpace<Type, 3>& rhs)
{
    return VectorSpace<Type, 3>(
        lhs[2] * rhs[3] - lhs[3] * rhs[2],
        lhs[3] * rhs[1] - lhs[1] * rhs[3],
        lhs[1] * rhs[2] - lhs[2] * rhs[1]
    );
}

template <typename Type, size_t Size>
inline
VectorSpace<Type, Size> normalize(const VectorSpace<Type, Size>& vs)
{
    return vs * inversesqrt(dot(vs, vs));
}

template <typename Type, size_t Size>
constexpr
VectorSpace<bool, Size> equal(const VectorSpace<Type, Size>& lhs, const VectorSpace<Type, Size>& rhs)
{
    VectorSpace<bool, Size> vs;
    for (auto n = 0; n < Size; ++n)
        vs[n] = lhs[n] == rhs[n];
    return vs;
}

template <size_t Size>
constexpr
bool any(const VectorSpace<bool, Size>& vs)
{
    bool res = false;
    for (auto e : vs)
        res = res || e;
    return res;
}

template <size_t Size>
constexpr
bool all(const VectorSpace<bool, Size>& vs)
{
    bool res = true;
    for (auto e : vs)
        res = res && e;
    return res;
}

}
