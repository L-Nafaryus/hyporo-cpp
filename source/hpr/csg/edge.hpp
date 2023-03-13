#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/vertex.hpp>


namespace hpr::csg
{

class Edge : public Shape
{

public:

    Edge() = default;

    ~Edge() override = default;

    explicit
    Edge(const Shape& shape) :
        Shape {shape.type() == Type::Edge ? shape : throw std::runtime_error("")}
    {}

    Edge(const vec3& p1, const vec3& p2) :
        Shape {BRepBuilderAPI_MakeEdge(gp_Pnt(p1[0], p1[1], p1[2]), gp_Pnt(p2[0], p2[1], p2[2])).Shape()}
    {}

    Edge(const Vertex& v1, const Vertex& v2) :
        Shape {BRepBuilderAPI_MakeEdge(v1.tcast(), v2.tcast()).Shape()}
    {}

    [[nodiscard]]
    TopoDS_Edge tcast() const
    {
        return TopoDS::Edge(p_shape);
    }
};

}
