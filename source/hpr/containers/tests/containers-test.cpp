#include <gtest/gtest.h>

#include "../array.hpp"


TEST(containers, StaticArray)
{
hpr::StaticArray<float, 3> arr {1, 3, 2};
hpr::StaticArray<float, 4> sarr {arr, 5};
hpr::StaticArray<float, 4> sarr2 {1, 3, 2, 5};
EXPECT_EQ(sarr, sarr2);
}

TEST(containers, DynamicArray)
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

hpr::DynamicArray<float*> arr4;
arr4.push(new float(5));
arr4.push(new float(7));
arr4.push(new float(9));
EXPECT_EQ(*arr4[0], 5.f);
EXPECT_EQ(*arr4[2], 9.f);
}
