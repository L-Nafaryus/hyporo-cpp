#pragma once

#include <hpr/math/integer.hpp>
#include <hpr/math/scalar.hpp>
#include <hpr/containers/array/static_array.hpp>
#include <hpr/math/quaternion.hpp>


namespace hpr
{

// forward declarations

template <IsReal Type, Size Rows, Size Cols> requires (Rows >= 0 && Cols >= 0)
class Matrix;

template <IsReal Type, Size Rows, Size Cols>
using SubMatrix = typename std::conditional<(Rows >= 2 && Cols >= 2), Matrix<Type, Rows - 1, Cols - 1>, Matrix<Type, 1, 1>>::type;

// type traits

template <typename T>
struct is_matrix : public std::false_type {};

template <typename T, Size Rows, Size Cols>
struct is_matrix<Matrix<T, Rows, Cols>> : public std::true_type {};

// concepts

template <typename T>
concept IsMatrix = is_matrix<T>::value;

}

namespace hpr
{

template <IsReal Type, Size Rows, Size Cols> requires (Rows >= 0 && Cols >= 0)
class Matrix : public StaticArray<Type, Rows * Cols>
{

    using base = StaticArray<Type, Rows * Cols>;

public:

    using value_type = Type;
    using size_type = Size;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator<Type>;
    using const_reference = Type const&;
    using const_iterator = Iterator<const Type>;

protected:

    size_type p_rows;
    size_type p_cols;

public:

    friend constexpr
    void swap(Matrix& main, Matrix& other)
    {
        using std::swap;
        swap(static_cast<base&>(main), static_cast<base&>(other));
        swap(main.p_rows, other.p_rows);
        swap(main.p_cols, other.p_cols);
    }

    inline
    Matrix() :
        base {},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(const Matrix& ms) :
        base {static_cast<base>(ms)},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(Matrix&& ms) noexcept:
        base {std::forward<base>(static_cast<base>(ms))},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix& operator=(const Matrix& ms)
    {
        //base::operator=(ms);
        swap(*this, ms);
        return *this;
    }

    inline explicit
    Matrix(const base& vs) :
            base {vs},
            p_rows {Rows},
            p_cols {Cols}
    {}

    inline explicit
    Matrix(base&& vs) noexcept:
        base {std::forward<base>(vs)},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(typename base::iterator start, typename base::iterator end) :
        base {start, end},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(std::initializer_list<value_type> list) :
        base {list},
        p_rows {Rows},
        p_cols {Cols}
    {}

    template <IsReal... Args>
    inline
    Matrix(value_type&& v, Args&& ...args) requires (1 + sizeof...(args) == Rows * Cols):
        base {v, static_cast<value_type>(std::forward<Args>(args))...},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    Matrix(const value_type& v) :
        base {},
        p_rows {Rows},
        p_cols {Cols}
    {
        for (Size n = 0; n < Rows * Cols; ++n)
            (*this)[n] = v;
    }

    inline
    Matrix& operator=(const value_type& v)
    {
        for (Size n = 0; n < Rows * Cols; ++n)
            (*this)[n] = v;
        return *this;
    }

    inline explicit
    Matrix(const Quaternion& q) requires (Rows == 3 && Cols == 3 || Rows == 4 && Cols == 4) :
        base {},
        p_rows {Rows},
        p_cols {Cols}
    {
        const scalar s = pow(norm(q), -2);

        (*this)(0, 0) = 1 - 2 * s * (q[2] * q[2] + q[3] * q[3]);
        (*this)(1, 0) = 2 * s * (q[1] * q[2] - q[3] * q[0]);
        (*this)(2, 0) = 2 * s * (q[1] * q[3] - q[2] * q[0]);

        (*this)(0, 1) = 2 * s * (q[1] * q[2] + q[3] * q[0]);
        (*this)(1, 1) = 1 - 2 * s * (q[1] * q[1] + q[3] * q[3]);
        (*this)(2, 1) = 2 * s * (q[2] * q[3] + q[1] * q[0]);

        (*this)(0, 2) = 2 * s * (q[1] * q[3] + q[2] * q[0]);
        (*this)(1, 2) = 2 * s * (q[2] * q[3] + q[1] * q[0]);
        (*this)(2, 2) = 1 - 2 * s * (q[1] * q[1] + q[2] * q[2]);

        if constexpr (Rows == 4)
            (*this)(3, 3) = 1;
    }


    // access

    inline
    reference operator()(size_type row, size_type col)
    {
        if (row >= p_rows || std::numeric_limits<size_type>::max() - p_rows < row)
            throw std::out_of_range("Row index is out of range");
        if (col >= p_cols || std::numeric_limits<size_type>::max() - p_cols < col)
            throw std::out_of_range("Column index is out of range");
        return (*this)[col + p_rows * row];
    }

    inline
    const_reference operator()(size_type row, size_type col) const
    {
        if (row >= p_rows || std::numeric_limits<size_type>::max() - p_rows < row)
            throw std::out_of_range("Row index is out of range");
        if (col >= p_cols || std::numeric_limits<size_type>::max() - p_cols < col)
            throw std::out_of_range("Column index is out of range");
        return (*this)[col + p_rows * row];
    }

    Vector<value_type, Cols> row(size_type row) const
    {
        Vector<value_type, Cols> vs;
        for (auto n = 0; n < Cols; ++n)
            vs[n] = (*this)(row, n);
        return vs;
    }

    void row(size_type row, const Vector<value_type, Cols>& vs)
    {
        for (auto n = 0; n < Cols; ++n)
            (*this)(row, n) = vs[n];
    }

    Vector<value_type, Rows> col(size_type col) const
    {
        Vector<value_type, Rows> vs;
        for (auto n = 0; n < Rows; ++n)
            vs[n] = (*this)(n, col);
        return vs;
    }

    void col(size_type col, const Vector<value_type, Rows>& vs)
    {
        for (auto n = 0; n < Rows; ++n)
            (*this)(n, col) = vs[n];
    }

    [[nodiscard]] constexpr size_type rows() const { return p_rows; }
    [[nodiscard]] constexpr size_type cols() const { return p_cols; }

    // member functions

    [[nodiscard]]
    constexpr
    bool is_square() const
    {
        return p_rows == p_cols;
    }

    inline
    Matrix& fill(value_type value)
    {
        for (auto n = 0; n < this->size(); ++n)
            (*this)[n] = value;
        return *this;
    }

    // Global functions

    static inline
    Matrix identity()
    {
        Matrix ms;
        for (auto n = 0; n < Rows; ++n)
            //for (auto k = 0; k < Cols; ++k)
                ms(n, n) = 1;
        return ms;
    }
};

// global operators

template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs) { Matrix<T, R, C> ms; for (Size n = 0; n < lhs.size(); ++n) ms[n] = lhs[n]; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs) { Matrix<T, R, C> ms; for (Size n = 0; n < lhs.size(); ++n) ms[n] = -lhs[n]; return ms; }

template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator+=(Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] += rhs[n]; return lhs; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator-=(Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] -= rhs[n]; return lhs; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator*=(Matrix<T, R, C>& lhs, const Matrix<T, C, R>& rhs) { Matrix<T, R, C> temp {lhs}; for (Size n = 0; n < R; ++n) for (Size k = 0; k < C; ++k) lhs(n, k) = sum(temp.col(k) * rhs.row(n)); return lhs; }

template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] += rhs[n]; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] -= rhs[n]; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator*(const Matrix<T, R, C>& lhs, const Matrix<T, C, R>& rhs) { Matrix<T, R, C> ms; for (Size n = 0; n < R; ++n) for (Size k = 0; k < C; ++k) ms(n, k) = sum(lhs.col(k) * rhs.row(n)); return ms; }

template <IsReal T, Size R, Size C> inline bool operator==(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { for (Size n = 0; n < lhs.size(); ++n) if (lhs[n] != rhs[n]) return false; return true; }
template <IsReal T, Size R, Size C> inline bool operator!=(const Matrix<T, R, C>& lhs, const Matrix<T, R, C>& rhs) { for (Size n = 0; n < lhs.size(); ++n) if (lhs[n] == rhs[n]) return false; return true; }


template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator+=(Matrix<T, R, C>& lhs, const T& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] += rhs; return lhs; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator-=(Matrix<T, R, C>& lhs, const T& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] -= rhs; return lhs; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator*=(Matrix<T, R, C>& lhs, const T& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] *= rhs; return lhs; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C>& operator/=(Matrix<T, R, C>& lhs, const T& rhs) { for (Size n = 0; n < lhs.size(); ++n) lhs[n] /= rhs; return lhs; }

template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator+(const Matrix<T, R, C>& lhs, const T& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] += rhs; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator-(const Matrix<T, R, C>& lhs, const T& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] -= rhs; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator*(const Matrix<T, R, C>& lhs, const T& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] *= rhs; return ms; }
template <IsReal T, Size R, Size C> inline Matrix<T, R, C> operator/(const Matrix<T, R, C>& lhs, const T& rhs) { Matrix<T, R, C> ms {lhs}; for (Size n = 0; n < lhs.size(); ++n) ms[n] /= rhs; return ms; }


template <IsReal T, Size R, Size C> inline Vector<T, R> operator*(const Matrix<T, R, C>& ms, const Vector<T, R>& vs) { Vector<T, R> res; for (Size n = 0; n < R; ++n) res[n] = sum(ms.row(n) * vs); return res; }
template <IsReal T, Size R, Size C> inline Vector<T, C> operator*(const Vector<T, R>& vs, const Matrix<T, R, C>& ms) { Vector<T, C> res; for (Size n = 0; n < C; ++n) res[n] = sum(ms.col(n) * vs); return res; }

template <IsReal T, Size R, Size C> inline bool operator==(const Matrix<T, R, C>& lhs, const Vector<T, R * C>& rhs) { return false; }
template <IsReal T, Size R, Size C> inline bool operator!=(const Matrix<T, R, C>& lhs, const Vector<T, R * C>& rhs) { return true; }

// matrix operations

//! Transpose matrix
template <IsReal T, Size R, Size C>
inline
Matrix<T, R, C> transpose(const Matrix<T, R, C>& ms)
{
    Matrix<T, R, C> res;
    for (Size n = 0; n < R; ++n)
        for (Size k = 0; k < C; ++k)
            res(k, n) = ms(n, k);
    return res;
}

//! Trace of a matrix
template <IsReal T, Size R, Size C>
inline
T trace(const Matrix<T, R, C>& ms) requires (R == C)
{
    T res;
    for (auto n = 0; n < R; ++n)
        res += ms(n, n);
    return res;
}

//! Minor of a matrix
template <IsReal T, Size R, Size C>
inline
SubMatrix<T, R, C> minor(const Matrix<T, R, C>& ms, Size row, Size col)
{
    if (ms.size() < 4)
        throw std::runtime_error("Matrix should be greater 2x2");

    SubMatrix<T, R, C> minor;
    auto minor_iter = minor.begin();
    for (auto n = 0; n < R; ++n)
        for (auto k = 0; k < C; ++k)
            if (k != col && n != row)
                *(minor_iter++) = ms[k + ms.rows() * n];
    return minor;
}

//! Determinant of a matrix
template <IsReal T, Size R, Size C>
inline
scalar det(const Matrix<T, R, C>& ms) requires (R == C)
{
    if (ms.size() == 1)
        return ms[0];

    else if (ms.size() == 4)
        return ms(0, 0) * ms(1, 1) - ms(0, 1) * ms(1, 0);

    else {
        scalar res = 0;
        for (auto n = 0; n < ms.cols(); ++n)
            res += pow(-1, n) * ms(0, n) * det(minor(ms, 0, n));
        return res;
    }
}

//! Adjoint matrix
template <IsReal T, Size R, Size C>
inline
Matrix<T, R, C> adj(const Matrix<T, R, C>& ms)
{
    Matrix<T, R, C> res;
    for (auto n = 0; n < R; ++n)
        for (auto k = 0; k < C; ++k)
            res(n, k) = pow(-1, n + k) * det(minor(ms, n, k));
    return transpose(res);
}

//! Inverse matrix
template <IsReal T, Size R, Size C>
inline
Matrix<T, R, C> inv(const Matrix<T, R, C>& ms)
{
    return adj(ms) / det(ms);
}

// Aliases

template <typename Type, size_t Row, size_t Col>
using mat = Matrix<Type, Row, Col>;

using mat2 = Matrix<scalar, 2, 2>;
using mat3 = Matrix<scalar, 3, 3>;
using mat4 = Matrix<scalar, 4, 4>;

}