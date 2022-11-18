#pragma once

#include "shape.hpp"
#include "edge.hpp"


namespace hpr::csg
{

class Wire : public Shape
{

public:

    Wire() = default;

    ~Wire() override = default;

    explicit
    Wire(const Shape& shape) :
        Shape {shape.type() == Type::Wire ? shape : throw std::runtime_error("")}
    {}

    explicit
    Wire(const darray<Shape>& edges) :
        Shape {}
    {
        BRepBuilderAPI_MakeWire builder;

        for (auto& shape : edges)
            switch (shape.type())
            {
                case Type::Edge:
                    builder.Add(Edge(shape).tcast());
                    break;
                case Type::Wire:
                    builder.Add(Wire(shape).tcast());
                    break;
                default:
                    throw std::runtime_error("");
            }

        p_shape = builder.Shape();
    }

    [[nodiscard]]
    TopoDS_Wire tcast() const
    {
        return TopoDS::Wire(p_shape);
    }
};

}
