#include <gtest/gtest.h>

#include <hpr/containers/array.hpp>
#include <hpr/containers/graph.hpp>


TEST(containers, StaticArray)
{
    hpr::StaticArray<float, 3> arr {7, 3, 2};
    hpr::StaticArray<float, 4> sarr {arr, 5};
    hpr::StaticArray<float, 4> sarr2 {7, 3, 2, 5};
    //hpr::StaticArray<float, 0> sarr4;

    EXPECT_EQ(sarr, sarr2);
    //EXPECT_EQ(sarr4.data(), nullptr);
    //EXPECT_EQ(sarr4.is_empty(), true);
    EXPECT_EQ(sarr.size(), 4);
    EXPECT_EQ(sarr2.size(), 4);
    EXPECT_EQ(sarr[0], 7);
    EXPECT_EQ(sarr[1], 3);
    EXPECT_EQ(sarr[2], 2);
    EXPECT_EQ(sarr[3], 5);
}

TEST(containers, DynamicArray)
{
    hpr::DynamicArray<float> arr {1, 3, 2};
    hpr::DynamicArray<float> arr2 {1, 3, 2};
    EXPECT_EQ(arr, arr2);
    EXPECT_TRUE(arr == arr2);
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
    arr3.insert(3, 19);
    EXPECT_EQ(arr3.size(), 6);
    EXPECT_EQ(arr3[3], 19);
    EXPECT_EQ(arr3.pop(), 5);
    EXPECT_EQ(arr3.size(), 5);
    arr3.resize(3);
    EXPECT_EQ(arr3.size(), 3);
    EXPECT_EQ(arr3.capacity(), 3);
    EXPECT_EQ(arr3.back(), 2);
    arr3.resize(4);
    EXPECT_EQ(arr3.back(), 2);
    arr3.push(17);
    arr3.push(14);
    EXPECT_EQ(arr3.back(), 14);
    EXPECT_EQ(arr3.size(), 5);

    arr3.clear();
    EXPECT_EQ(arr3.is_empty(), true);



    hpr::DynamicArray<float*> arr4;
    arr4.push(new float(5));
    arr4.push(new float(7));
    arr4.push(new float(9));
    EXPECT_EQ(*arr4[0], 5.f);
    EXPECT_EQ(*arr4[2], 9.f);
    for (auto& v : arr4)
        delete v;


}

TEST(containers, TreeNode)
{
    hpr::TreeNode<float> node1 (5);
    //std::shared_ptr<hpr::TreeNode<float>> ptr {node1};
    hpr::TreeNode<float> node2 {7, {&node1}};
    hpr::TreeNode<float> node3 {9, {&node2, &node1}};
    hpr::TreeNode<float> node4 {11, {&node3}};

    EXPECT_EQ(*node1.data(), 5);
    //EXPECT_EQ(*node1->data(), *node2.ancestor()->data());

    hpr::darray<hpr::TreeNode<float>*> tr = node1.traverse_descendants();
    EXPECT_EQ(tr.size(), 0);
    hpr::darray<hpr::TreeNode<float>*> tr2 = node4.traverse_descendants();
    EXPECT_EQ(tr2.size(), 4);
    hpr::darray<hpr::TreeNode<float>*> tr3 = node1.traverse_ancestors();
    EXPECT_EQ(tr3.size(), 2); // node1 has changed ancestor
    //
}
