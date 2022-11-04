#include <gtest/gtest.h>

#include "array.hpp"
#include "vector.hpp"
#include "matrix.hpp"


TEST(hyplib, Array)
{
    hpr::StaticArray<float, 3> arr {1, 3, 2};
    hpr::StaticArray<float, 4> sarr {arr, 5};
    hpr::StaticArray<float, 4> sarr2 {1, 3, 2, 5};
    EXPECT_EQ(sarr, sarr2);
}

TEST(hyplib, DynamicArray)
{
    hpr::DynamicArray<float> arr {1, 3, 2};
    hpr::DynamicArray<float> arr2 {1, 3, 2};
    EXPECT_EQ(arr, arr2);
    arr.remove(1);
    EXPECT_EQ(arr, hpr::darray<float>({1, 2}));
    auto iter = arr2.begin();
    ++iter;
    arr2.remove(iter);
    EXPECT_EQ(arr2, hpr::darray<float>({1, 2}));

    hpr::DynamicArray<float> arr3 {1, 3, 0, 2, 9, 0, 5};
    arr3.remove([](float num) { return num == 0; });
    EXPECT_EQ(arr3, hpr::darray<float>({1, 3, 2, 9, 5}));
    EXPECT_EQ(arr3.size(), 5);
}

TEST(hyplib, Vector)
{
    hpr::vec3 v1 {1, 3, 2};
    hpr::vec3 v2 {5, 7, -1};
    hpr::vec2 v31 {13, -2};
    hpr::vec3 v32 {v31, 9};
    EXPECT_EQ((hpr::vec3(v2.begin(), v2.end())), v2);
    EXPECT_EQ(v32, hpr::vec3(13, -2, 9));
    EXPECT_EQ(-v1, hpr::vec3(-1, -3, -2));
    EXPECT_EQ(v1 + v2, hpr::vec3(6, 10, 1));
    EXPECT_EQ(v1 - v2, hpr::vec3(-4, -4, 3));
    EXPECT_EQ((hpr::dot(v1, v2) ), 24);
}

TEST(hyplib, Matrix)
{
    hpr::mat2 m1;
    hpr::vec4 v1;
    EXPECT_FALSE(v1 == m1);
    hpr::mat2 m2 {3, 2, 7, 4};
    hpr::vec2 v2 {2, 4};
    EXPECT_EQ(m2.col(1), v2);
    hpr::vec2 v3 {13, 51};
    m2.col(1, v3);
    EXPECT_EQ(m2.col(1), v3);
    hpr::mat3 m4 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    hpr::mat2 m41 {5, 6, 8, 9};
    EXPECT_EQ(m41.minor(0, 0), 9);
    hpr::mat2 m5 {1, 2, 3, 4};

    EXPECT_EQ((m4.det()), 0);
    EXPECT_EQ(hpr::mat3(-9, 23, 3, 5, 5, 6, 7, -3, 9).det(), -786);

    hpr::mat2 m6 {2, 1, 7, 4};
    EXPECT_EQ(m6.inv(), hpr::mat2(4, -1, -7, 2));
    //EXPECT_EQ(m4.det(), 0);
}
/*using Minor = typename std::conditional<(Rows > 2 && Cols > 2),
        MatrixSpace<Type, Rows - 1, Cols - 1>, MatrixSpace<Type, 1, 1>>::type;

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
        for (auto m = 0; m < Cols; ++m) {
            Minor minor;
            auto minor_iter = minor.begin();
            for (auto n = 0; n < Rows; ++n)
                for (auto k = 0; k < Cols; ++k)
                    if (k != 0 && n != m)
                        *(minor_iter++) = (*this)[k + p_rows * n];
            res += pow(-1, m) * (*this)(0, m) * minor.det();
        }
        return res;
    }
}
*/