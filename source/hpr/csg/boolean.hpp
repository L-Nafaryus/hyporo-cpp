#pragma once

#include <hpr/csg/shape.hpp>


namespace hpr::csg
{

Shape fuse(const darray<Shape>& args, const darray<Shape>& tools)
{
    BRepAlgoAPI_Fuse builder;
    NCollection_List<TopoDS_Shape> args_, tools_;

    for (const auto& arg : args)
        args_.Append(arg.tshape());
    for (const auto& tool : tools)
        tools_.Append(tool.tshape());

    builder.SetArguments(args_);
    builder.SetTools(tools_);
    builder.Build();

    return Shape {builder.Shape()};
}

Shape fuse(const Shape& lhs, const Shape& rhs)
{
    BRepAlgoAPI_Fuse builder {lhs.tshape(), rhs.tshape()};
    builder.Build();

    return Shape {builder.Shape()};
}

Shape common(const Shape& lhs, const Shape& rhs)
{
    BRepAlgoAPI_Common builder {lhs.tshape(), rhs.tshape()};
    builder.Build();

    return Shape {builder.Shape()};
}

Shape cut(const Shape& lhs, const Shape& rhs)
{
    BRepAlgoAPI_Cut builder {lhs.tshape(), rhs.tshape()};
    builder.Build();

    return Shape {builder.Shape()};
}

}
