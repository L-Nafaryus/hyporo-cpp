#pragma once

#include <hpr/csg/shape.hpp>


namespace hpr::csg
{

class Vertex : public Shape
{

public:

    Vertex() = default;

    ~Vertex() override = default;

    explicit
    Vertex(const Shape& shape) :
        Shape {shape.type() == Type::Vertex ? shape : throw std::runtime_error("")}
    {}

    explicit
    Vertex(const vec3& point) :
        Shape {BRepBuilderAPI_MakeVertex(gp_Pnt(point[0], point[1], point[2])).Shape()}
    {}

    [[nodiscard]]
    TopoDS_Vertex tcast() const
    {
        return TopoDS::Vertex(p_shape);
    }

    [[nodiscard]]
    vec3 cast() const
    {
        gp_Pnt point = BRep_Tool::Pnt(tcast());
        return vec3 {point.X(), point.Y(), point.Z()};
    }
};

}

