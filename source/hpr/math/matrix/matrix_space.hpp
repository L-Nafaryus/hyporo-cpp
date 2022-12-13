#pragma once

#include "../vector.hpp"


namespace hpr
{

template <typename Type, size_t Rows, size_t Cols>
class MatrixSpace : public VectorSpace<Type, Rows * Cols>
{
    static_assert(Rows >= 1);
    static_assert(Cols >= 1);
    using base = VectorSpace<Type, Rows * Cols>;

    using Minor = typename std::conditional<(Rows >= 2 && Cols >= 2), MatrixSpace<Type, Rows - 1, Cols - 1>, MatrixSpace<Type, 1, 1>>::type;
public:

    using value_type = Type;
    using size_type = size_t;
    using pointer = Type*;
    using reference = Type&;
    using iterator = Iterator<Type>;
    using const_reference = Type const&;
    using const_iterator = Iterator<const Type>;

protected:

    static
    const size_type p_mrows = Rows - 1;
    static
    const size_type p_mcols = Cols - 1;
    size_type p_rows;
    size_type p_cols;

public:

    inline
    MatrixSpace() :
        base {},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    MatrixSpace(const MatrixSpace& ms) :
        base {static_cast<base>(ms)},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline explicit
    MatrixSpace(const base& ms) :
        base {ms},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    MatrixSpace(MatrixSpace&& ms) noexcept :
        base {std::move(static_cast<base>(ms))},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    MatrixSpace& operator=(const MatrixSpace& ms)
    {
        base::operator=(ms);
        return *this;
    }

    inline
    MatrixSpace(typename base::iterator start, typename base::iterator end) :
        base {start, end},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    MatrixSpace(typename base::const_iterator start, typename base::const_iterator end) :
        base {start, end},
        p_rows {Rows},
        p_cols {Cols}
    {}

    inline
    MatrixSpace(std::initializer_list<value_type> list) :
        base {list},
        p_rows {Rows},
        p_cols {Cols}
    {}

    template <std::convertible_to<value_type>... Args>
    inline
    MatrixSpace(value_type&& v, Args&& ...args) :
        base {v, static_cast<value_type>(std::forward<Args>(args))...},
        p_rows {Rows},
        p_cols {Cols}
    {
        static_assert(1 + sizeof...(args) == Rows * Cols, "Number of arguments must be equal to rows * cols of matrix");
    }

    // Member functions

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

    VectorSpace<value_type, Cols> row(size_type row)
    {
        VectorSpace<value_type, Cols> vs;
        for (auto n = 0; n < Cols; ++n)
            vs[n] = (*this)(row, n);
        return vs;
    }

    VectorSpace<value_type, Cols> row(size_type row) const
    {
        VectorSpace<value_type, Cols> vs;
        for (auto n = 0; n < Cols; ++n)
            vs[n] = (*this)(row, n);
        return vs;
    }

    void row(size_type row, const VectorSpace<value_type, Cols>& vs)
    {
        for (auto n = 0; n < Cols; ++n)
            (*this)(n, row) = vs[n];
    }

    VectorSpace<value_type, Rows> col(size_type col)
    {
        VectorSpace<value_type, Rows> vs;
        for (auto n = 0; n < Rows; ++n)
            vs[n] = (*this)(n, col);
        return vs;
    }

    void col(size_type col, const VectorSpace<value_type, Rows>& vs)
    {
        for (auto n = 0; n < Rows; ++n)
            (*this)(n, col) = vs[n];
    }

    size_type rows() const { return p_rows; }
    size_type cols() const { return p_cols; }

    [[nodiscard]]
    inline
    bool is_square() const
    {
        return p_rows == p_cols;
    }

    inline
    Minor minor(size_type row, size_type col)
    {
        if (this->size() < 4)
            throw std::runtime_error("Impossible to find minor for matrix with size less than 2x2");
        Minor minor;
        auto minor_iter = minor.begin();
        for (auto n = 0; n < Rows; ++n)
            for (auto k = 0; k < Cols; ++k)
                if (k != col && n != row)
                    *(minor_iter++) = (*this)[k + p_rows * n];
        return minor;
    }

    inline
    value_type det()
    {
        if (!is_square())
            throw std::runtime_error("Matrix must be square");
        if (this->size() == 1)
            return (*this)[0];
        else if (this->size() == 4)
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
        else {
            auto res = 0;
            for (auto m = 0; m < Cols; ++m)
                res += std::pow(-1, m) * (*this)(0, m) * minor(0, m).det();
            return res;
        }
    }

    inline
    MatrixSpace transpose()
    {
        MatrixSpace ms;
        for (auto n = 0; n < Rows; ++n)
            for (auto k = 0; k < Cols; ++k)
                ms(k, n) = (*this)(n, k);
        return ms;
    }

    inline
    MatrixSpace adj()
    {
        MatrixSpace ms;
        for (auto n = 0; n < Rows; ++n)
            for (auto k = 0; k < Cols; ++k)
            {
                ms(n, k) = std::pow(-1, n + k) * minor(n, k).det();
            }
        return ms.transpose();
    }

    inline
    MatrixSpace inv()
    {
        return MatrixSpace(adj() / det());
    }

    // Friend functions

    friend inline
    bool operator==(const MatrixSpace& lhs, const MatrixSpace& rhs)
    {
        for (auto n = 0; n < lhs.size(); ++n)
            if (lhs[n] != rhs[n])
                return false;
        return true;
    }

    friend inline
    bool operator!=(const MatrixSpace& lhs, const MatrixSpace& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, size_type S>
    friend inline
    bool operator==(const MatrixSpace& lhs, const VectorSpace<T, S>& rhs)
    {
        return false;
    }

    friend inline
    VectorSpace<Type, Cols> operator*(const VectorSpace<Type, Cols>& vs, const MatrixSpace& ms)
    {
        VectorSpace<Type, Cols> res;
        for (auto n = 0; n < Cols; ++n)
            res[0] = sum(ms.col(n) * vs);
        return res;
    }

    friend inline
    VectorSpace<Type, Rows> operator*(const MatrixSpace& ms, const VectorSpace<Type, Rows>& vs)
    {
        VectorSpace<Type, Rows> res;
        for (auto n = 0; n < Rows; ++n)
            res[0] = sum(ms.row(n) * vs);
        return res;
    }

    MatrixSpace& fill(value_type value)
    {
        for (auto n = 0; n < this->size(); ++n)
            (*this)[n] = value;
        return *this;
    }

    // Global functions

    static inline
    MatrixSpace identity()
    {
        MatrixSpace ms;
        for (auto n = 0; n < Rows; ++n)
            for (auto k = 0; k < Cols; ++k)
                ms(n, k) = 1;
        return ms;
    }
};

// Aliases

template <typename Type, size_t Row, size_t Col>
using mat = MatrixSpace<Type, Row, Col>;

using mat2 = MatrixSpace<scalar, 2, 2>;
using mat3 = MatrixSpace<scalar, 3, 3>;
using mat4 = MatrixSpace<scalar, 4, 4>;

}