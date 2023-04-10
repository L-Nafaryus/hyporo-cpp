#pragma once

#include <hpr/csg/shape.hpp>


namespace hpr::csg
{

Shape extrude(const Shape& shape, const vec3& dir, double length)
{
    BRepPrimAPI_MakePrism builder {shape.tshape(), length * gp_Vec(dir[0], dir[1], dir[2]), true};
    builder.Build();
    return builder.Shape();
}

Shape fillet(const Shape& shape, const darray<Shape>& edges, double radius)
{
    BRepFilletAPI_MakeFillet fillet {shape.tshape()};

    for (auto& e : edges)
        fillet.Add(radius, TopoDS::Edge(e.tshape()));
    fillet.Build();

    return fillet.Shape();
}

Shape sphere(const vec3& center, double radius)
{
    BRepPrimAPI_MakeSphere prim {gp_Pnt(center[0], center[1], center[2]), radius};
    return Shape {prim.Shape()};
}

Shape box(const vec3& corner, double dx, double dy, double dz)
{
    BRepPrimAPI_MakeBox prim {gp_Pnt(corner[0], corner[1], corner[2]), dx, dy, dz};
    return Shape {prim.Shape()};
}

Shape box(const vec3& corner, const vec3& scale)
{
    return box(corner, scale[0], scale[1], scale[2]);
}
}