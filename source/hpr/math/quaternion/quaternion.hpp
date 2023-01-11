#pragma once

#include "../vector.hpp"


namespace hpr
{

// Forward declaration

class Quaternion;

inline
Quaternion inverse(const Quaternion& q);

inline
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

// Class declaration
class Quaternion
{

public:

    enum RotationSequence
    {
        ZYX, ZYZ, ZXY, ZXZ, YXZ, YXY, YZX, YZY, XYZ, XYX, XZY, XZX
    };

protected:

    scalar p_real;
    vec3 p_imag;

public:

    inline
    Quaternion() :
        p_real{},
        p_imag{}
    {}

    inline
    Quaternion(const scalar real, const vec3& imag) :
        p_real {real},
        p_imag {imag}
    {}

    inline explicit
    Quaternion(const scalar real) :
        p_real {real},
        p_imag {}
    {}

    inline explicit
    Quaternion(const vec3& imag) :
        p_real {},
        p_imag {imag}
    {}

    inline
    Quaternion(const vec3& vs, const scalar& theta) :
        p_real {cos(0.5 * theta)},
        p_imag {sin(0.5 * theta) * vs / mag(vs)}
    {}

    static inline
    Quaternion unit(const vec3& vs)
    {
        return Quaternion(sqrt(1 - norm(vs)), vs);
    }

    inline
    Quaternion(const RotationSequence rs, const vec3& angles)
    {
        switch (rs)
        {
            case XYZ:
                *this = Quaternion(vec3(0, 1, 0), angles[0]) *
                        Quaternion(vec3(0, 1, 0), angles[1]) *
                        Quaternion(vec3(0, 0, 1), angles[2]);
                break;

            default:
                throw std::runtime_error("Unknown rotation sequence");
        }
    }

    inline
    scalar real() const
    {
        return p_real;
    }

    inline
    scalar& real()
    {
        return p_real;
    }

    inline
    vec3 imag() const
    {
        return p_imag;
    }

    inline
    vec3& imag()
    {
        return p_imag;
    }

    inline
    void operator+=(const Quaternion& q)
    {
        p_real += q.p_real;
        p_imag += q.p_imag;
    }

    inline
    void operator-=(const Quaternion& q)
    {
        p_real -= q.p_real;
        p_imag -= q.p_imag;
    }

    inline
    void operator*=(const Quaternion& q)
    {
        scalar temp = p_real;
        p_real = p_real * q.p_real - dot(p_imag, q.p_imag);
        p_imag = temp * q.p_imag + q.p_real * p_imag + cross(p_imag, q.p_imag);
    }

    inline
    void operator/=(const Quaternion& q)
    {
        operator*=(inverse(q));
    }

    inline
    void operator*=(const scalar s)
    {
        p_real *= s;
        p_imag *= s;
    }

    inline
    void operator/=(const scalar s)
    {
        p_real /= s;
        p_imag /= s;
    }
};

inline
bool equal(const Quaternion& lhs, const Quaternion& rhs)
{
    return lhs.real() == rhs.real() && lhs.imag() == rhs.imag();
}

inline
bool operator==(const Quaternion& lhs, const Quaternion& rhs)
{
    return equal(lhs, rhs);
}

inline
bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
{
    return !equal(lhs, rhs);
}

inline
Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
{
    return {lhs.real() + rhs.real(), lhs.imag() + rhs.imag()};
}

inline
Quaternion operator-(const Quaternion& q)
{
    return {q.real(), q.imag()};
}

inline
Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)
{
    return {lhs.real() - rhs.real(), lhs.imag() - rhs.imag()};
}

inline
Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
{
    return {lhs.real() * rhs.real() - dot(lhs.imag(), rhs.imag()),
        lhs.real() * rhs.imag() + rhs.real() * lhs.imag() + cross(lhs.imag(), rhs.imag())};
}

inline
Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs)
{
    return lhs * inverse(rhs);
}

inline
Quaternion operator*(const scalar s, const Quaternion& q)
{
    return {s * q.real(), s * q.imag()};
}

inline
Quaternion operator*(const Quaternion& q, const scalar s)
{
    return {q.real() * s, q.imag() * s};
}

inline
Quaternion operator/(const Quaternion& q, const scalar s)
{
    return {q.real() / s, q.imag() / s};
}

inline
scalar norm(const Quaternion& q)
{
    return sqrt(pow(q.real(), 2) + dot(q.imag(), q.imag()));
}

inline
Quaternion conjugate(const Quaternion& q)
{
    return {q.real(), -q.imag()};
}

inline
Quaternion inverse(const Quaternion& q)
{
    return conjugate(q) / pow(norm(q), 2);
}

inline
Quaternion normalize(const Quaternion& q)
{
    return q / norm(q);
}

inline
vec3 rotate(const vec3& point, const vec3& axis, const scalar& angle)
{
    Quaternion p {point};
    Quaternion q {normalize(axis), angle};
    return (q * p * inverse(q)).imag();
}

// Aliases

using quat = Quaternion;

}