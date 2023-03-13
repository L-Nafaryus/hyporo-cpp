#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/solid.hpp>


namespace hpr::csg
{

    class Compound : public Shape
    {

    public:

        Compound() = default;

        ~Compound() override = default;

        explicit
        Compound(const Shape& shape) :
                Shape {shape.type() == Type::Compound ? shape : throw std::runtime_error("")}
        {}

        explicit
        Compound(const darray<Shape>& shapes) :
                Shape {}
        {
            BRep_Builder builder;
            TopoDS_Compound compound;
            builder.MakeCompound(compound);

            for (auto& shape : shapes)
                builder.Add(compound, shape.tshape());

            p_shape = compound;
        }

        [[nodiscard]]
        TopoDS_Compound tcast() const
        {
            return TopoDS::Compound(p_shape);
        }
    };

}
