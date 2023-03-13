#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/shell.hpp>


namespace hpr::csg
{

    class Solid : public Shape
    {

    public:

        Solid() = default;

        ~Solid() override = default;

        explicit
        Solid(const Shape& shape) :
                Shape {shape.type() == Type::Solid ? shape : throw std::runtime_error("")}
        {}

        explicit
        Solid(const Shell& shell) :
            Shape {}
        {
            BRep_Builder builder;
            TopoDS_Solid solid;

            builder.MakeSolid(solid);
            builder.Add(solid, shell.tcast());

            p_shape = solid;
        }

        [[nodiscard]]
        TopoDS_Solid tcast() const
        {
            return TopoDS::Solid(p_shape);
        }
    };

}

