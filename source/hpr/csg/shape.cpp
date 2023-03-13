#include <hpr/csg/shape.hpp>


bool std::less<hpr::csg::Shape>::operator()(const hpr::csg::Shape& s1, const hpr::csg::Shape& s2) const
{
    return s1.tshape().HashCode(std::numeric_limits<Standard_Integer>::max()) <
           s2.tshape().HashCode(std::numeric_limits<Standard_Integer>::max());
}
namespace hpr::csg
{

std::map<Shape, Shape::Metadata> Shape::metadata;


Shape sphere(vec3 center, double radius)
{
    BRepPrimAPI_MakeSphere prim {gp_Pnt(center[0], center[1], center[2]), radius};
    return Shape {prim.Shape()};
}

Shape box(vec3 corner, double dx, double dy, double dz)
{
    BRepPrimAPI_MakeBox prim {gp_Pnt(corner[0], corner[1], corner[2]), dx, dy, dz};
    return Shape {prim.Shape()};
}

}
