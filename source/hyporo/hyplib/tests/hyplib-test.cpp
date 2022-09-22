#include <gtest/gtest.h>
#include "vector.hpp"

TEST(hyplibTest, VectorCreation)
{
    hyporo::vec3 v {1, 3, 2};
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 2);
}

TEST(hyplibTest, VectorOperations)
{
    using hyporo::vec3;
    vec3 v1 {1, 3, 2};
    vec3 v2 {5, 7, -1};
    EXPECT_EQ(-v1, vec3(-1, -3, -2));
    EXPECT_EQ(v1 + v2, vec3(6, 10, 1));
    EXPECT_EQ(v1 - v2, vec3(-4, -4, 3));
}
