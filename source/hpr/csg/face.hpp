#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/wire.hpp>


namespace hpr::csg
{

class Face : public Shape
{

public:

    Face() = default;

    ~Face() override = default;

    explicit
    Face(const Shape& shape) :
        Shape {shape.type() == Type::Face ? shape : throw std::runtime_error("")}
    {}

    explicit
    Face(const Wire& wire) :
        Shape {BRepBuilderAPI_MakeFace(wire.tcast()).Shape()}
    {}

    explicit
    Face(const darray<Edge>& edges) :
        Face {Wire(edges)}
    {}

    [[nodiscard]]
    TopoDS_Face tcast() const
    {
        return TopoDS::Face(p_shape);
    }
};

}

