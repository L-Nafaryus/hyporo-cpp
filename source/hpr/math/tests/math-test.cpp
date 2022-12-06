#include <gtest/gtest.h>

#include "../vector.hpp"
#include "../matrix.hpp"


TEST(math, Vector)
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
    EXPECT_EQ((hpr::cross(hpr::vec3(1, 0, 0), hpr::vec3(0, 1, 0))), hpr::vec3(0, 0, 1));
    EXPECT_EQ((hpr::angle(hpr::vec3(1, 0, 0), hpr::vec3(0, 0, 1))), hpr::PI * 0.5);
    EXPECT_EQ((hpr::normalize(hpr::vec3(1, 1, 1))), hpr::vec3(0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
}

TEST(math, Matrix)
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
    EXPECT_EQ(hpr::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1).det(), 1.);
    //EXPECT_EQ(m4.det(), 0);
}