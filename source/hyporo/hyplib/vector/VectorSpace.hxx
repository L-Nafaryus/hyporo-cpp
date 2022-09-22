
#include <algorithm>


namespace hyporo
{

// Constructors

template <class C, sizet NC>
inline VectorSpace<C, NC>::VectorSpace()
{}

template <class C, sizet NC>
template <class... Args>
inline VectorSpace<C, NC>::VectorSpace(const Args... components)
    : row { static_cast<C>(components)... }
{}


// Member functions

template <class C, sizet NC>
inline sizet VectorSpace<C, NC>::size()
{
    return NC;
}

// Member operators

template <class C, sizet NC>
inline C& VectorSpace<C, NC>::operator[](const sizet n)
{
    return row[n];
}


template <class C, sizet NC>
inline VectorSpace<C, NC>& VectorSpace<C, NC>::operator*=(const scalar s)
{
    for (C& v : row)
        v *= s;
    return *this;
}

template <class C, sizet NC>
inline VectorSpace<C, NC>& VectorSpace<C, NC>::operator/=(const scalar s)
{
    for (C& v : row)
        v /= s;
    return *this;
}

// Friend operators

template <class C, sizet NC>
std::ostream& operator<<(std::ostream& os, const VectorSpace<C, NC>& vs)
{
    os << "(";
    for (sizet n = 0; n < NC; n++)
    {
        os << vs.row[n];
        if (n != NC - 1)
            os << ", ";
    }
    os << ")";
    return os;
}


// Global operators

template <class C, sizet NC>
inline VectorSpace<C, NC> operator-(const VectorSpace<C, NC>& vs)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = -vs.row[n];
    return nvs;
}

template <class C, sizet NC>
inline VectorSpace<C, NC> operator+(const VectorSpace<C, NC>& vs1, const VectorSpace<C, NC>& vs2)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = vs1.row[n] + vs2.row[n];
    return nvs;
}

template <class C, sizet NC>
inline VectorSpace<C, NC> operator-(const VectorSpace<C, NC>& vs1, const VectorSpace<C, NC>& vs2)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = vs1.row[n] - vs2.row[n];
    return nvs;
}

template <class C, sizet NC>
inline VectorSpace<C, NC> operator*(const scalar& s, const VectorSpace<C, NC>& vs)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = s * vs.row[n];
    return nvs;
}

template <class C, sizet NC>
inline VectorSpace<C, NC> operator*(const VectorSpace<C, NC>& vs, const scalar& s)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = vs.row[n] * s;
    return nvs;
}

template <class C, sizet NC>
inline VectorSpace<C, NC> operator/(const VectorSpace<C, NC>& vs, const scalar& s)
{
    VectorSpace<C, NC> nvs;
    for (sizet n = 0; n < NC; n++)
        nvs.row[n] = vs.row[n] / s;
    return nvs;
}

template <class C, sizet NC>
inline bool operator==(const VectorSpace<C, NC>& vs1, const VectorSpace<C, NC>& vs2)
{
    bool eq = true;
    for (sizet n = 0; n < NC; n++)
        if (!(eq &= (vs1.row[n] == vs2.row[n])))
            break;
    return eq;
}

template <class C, sizet NC>
inline bool operator!=(const VectorSpace<C, NC>& vs1, const VectorSpace<C, NC>& vs2)
{
    return !(vs1 == vs2);
}

} // end namespace hyporo
