#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/face.hpp>


namespace hpr::csg
{

class Shell : public Shape
{

public:

    Shell() = default;

    ~Shell() override = default;

    explicit
    Shell(const Shape& shape) :
        Shape {shape.type() == Type::Shell ? shape : throw std::runtime_error("")}
    {}

    explicit
    Shell(const darray<Shape>& faces) :
        Shape {}
    {
        BRep_Builder builder;
        TopoDS_Shell shell;
        builder.MakeShell(shell);

        for (auto& shape : faces)
            switch (shape.type())
            {
                case Type::Face:
                    builder.Add(shell, Face(shape).tcast());
                    break;
                default:
                    throw std::runtime_error("");
            }

        p_shape = shell;
    }

    [[nodiscard]]
    TopoDS_Shell tcast() const
    {
        return TopoDS::Shell(p_shape);
    }
};

}
