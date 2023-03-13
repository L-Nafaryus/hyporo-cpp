#include <gtest/gtest.h>

#include <hpr/math/vector.hpp>
#include <hpr/math/matrix.hpp>
#include <hpr/math/quaternion.hpp>

#include <complex>


TEST(math, Scalar)
{
    EXPECT_EQ(hpr::scalar(5) + hpr::scalar(7), hpr::scalar(12));
    EXPECT_TRUE(std::is_floating_point_v<hpr::Scalar<double>>);
    EXPECT_TRUE(std::is_arithmetic_v<hpr::Scalar<double>>);
    //EXPECT_EQ(5.f, hpr::Scalar<double>(5));
    EXPECT_EQ(hpr::rad(180), hpr::pi());
    EXPECT_EQ(hpr::deg(hpr::pi()), 180);
    EXPECT_EQ(hpr::cos(0), 1);
    EXPECT_EQ(hpr::sin(0), 0);
    EXPECT_EQ(hpr::abs(hpr::scalar(-1)), 1);
    EXPECT_EQ(hpr::pow(2, 2), 4);
    EXPECT_EQ(hpr::precision<hpr::scalar>(), static_cast<hpr::scalar>(1e-15));

    EXPECT_TRUE(typeid(static_cast<float>(hpr::scalar(5))) == typeid(float));
    EXPECT_FALSE(!hpr::scalar(-1.));

    std::stringstream oss;
    oss << hpr::cos(0);
    EXPECT_EQ(oss.str(), "1");
    hpr::scalar s;
    oss >> s;
    EXPECT_EQ(s, 1);

    EXPECT_TRUE(hpr::equal(5.5453535353535395818593, 5.5453535353535395817592, 1e-18));
    EXPECT_EQ(hpr::min(7., 5), 5);
    EXPECT_EQ(hpr::max(7., 5), 7);
    EXPECT_EQ(hpr::clip(7., 5, 10), 7);
    EXPECT_EQ(hpr::clip(1., 5, 10), 5);
    EXPECT_EQ(hpr::clip(72., 5, 10), 10);
}

TEST(math, Vector)
{
    hpr::vec3 v1 {1, 3, 2};
    hpr::vec3 v2 {5, 7, -1};
    hpr::vec2 v31 {13, -2};
    hpr::vec3 v32 (v31, 9);
    EXPECT_EQ((hpr::vec3(v2.begin(), v2.end())), v2);
    EXPECT_EQ(v32, hpr::vec3(13, -2, 9));
    EXPECT_EQ(-v1, hpr::vec3(-1, -3, -2));
    EXPECT_EQ(v1 + v2, hpr::vec3(6, 10, 1));
    EXPECT_EQ(v1 - v2, hpr::vec3(-4, -4, 3));
    EXPECT_EQ((hpr::dot(v1, v2) ), 24);
    EXPECT_EQ((hpr::cross(hpr::vec3(1, 0, 0), hpr::vec3(0, 1, 0))), hpr::vec3(0, 0, 1));
    EXPECT_EQ((hpr::angle(hpr::vec3(1, 0, 0), hpr::vec3(0, 0, 1))), hpr::pi() * 0.5);
    EXPECT_EQ((hpr::normalize(hpr::vec3(1, 1, 1))), hpr::vec3(0.5773502691896258, 0.5773502691896258, 0.5773502691896258));
}

TEST(math, Matrix)
{
    hpr::mat2 m1;
    hpr::vec4 v1;
    EXPECT_FALSE(v1 == m1);
    EXPECT_FALSE(m1 == v1);
    hpr::mat2 m2 {3, 2, 7, 4};
    hpr::vec2 v2 {2, 4};
    EXPECT_EQ(m2.col(1), v2);
    hpr::vec2 v3 {13, 51};
    m2.col(1, v3);
    EXPECT_EQ(m2.col(1), v3);
    hpr::mat3 m4 {1, 2, 3, 4, 5, 6, 7, 8, 9};
    hpr::mat2 m41 {5, 6, 8, 9};
    //EXPECT_EQ(minor(m41, 0, 0), 9);
    hpr::mat2 m5 {1, 2, 3, 4};

    EXPECT_EQ(det(m4), 0);
    EXPECT_EQ(hpr::det(m4), 0);
    EXPECT_EQ(det(hpr::mat3(-9, 23, 3, 5, 5, 6, 7, -3, 9)), -786);

    hpr::mat2 m6 {2, 1, 7, 4};
    EXPECT_EQ(det(m6), 1);
    EXPECT_EQ(adj(m6), hpr::mat2(4, -1, -7, 2));
    EXPECT_EQ(inv(m6), hpr::mat2(4, -1, -7, 2));
    EXPECT_EQ(det(hpr::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1)), 1.);
    //EXPECT_EQ(m4.det(), 0);
}

TEST(math, Quaternion)
{
    hpr::quat q;
    hpr::vec3 np = hpr::rotate(hpr::vec3(0, 1, 0), {1, 0, 0}, hpr::pi() * 0.5);
    EXPECT_TRUE(hpr::all(hpr::equal(np, hpr::vec3(0, 0, 1))));
    EXPECT_EQ(hpr::norm(hpr::quat(np)), 1);
}