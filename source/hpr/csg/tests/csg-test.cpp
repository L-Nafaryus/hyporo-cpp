#include <gtest/gtest.h>
#include "../../csg.hpp"

TEST(csgTest, Shape)
{
    using namespace hpr;
    double radius = 1.;
    double volume = 4. / 3. * PI;
    auto sphere = csg::sphere({0, 0, 0}, radius);
    EXPECT_TRUE(equal(sphere.volume(), volume, 1e-6));
    auto box = csg::box({0, 0, 0}, 1, 1, 1);
    EXPECT_TRUE(equal(box.volume(), 1.));
    auto edge = csg::Edge();
    int n = 0;
    for (auto& face : box.subShapes(csg::Shape::Type::Face))
    {
        std::stringstream name;
        name << "face" << n;
        csg::Face(face).label(name.str());
        ++n;
    }
    box.scale(box.center(), 5);
    EXPECT_EQ(box.subShapes(csg::Shape::Type::Face)[2].label(), "face2");
}