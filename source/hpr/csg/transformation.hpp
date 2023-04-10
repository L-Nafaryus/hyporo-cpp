#pragma once

#include <hpr/csg/shape.hpp>
#include <hpr/csg/compound.hpp>


namespace hpr::csg
{

Shape scale(const Shape& shape, const vec3& center, double scale)
{
    gp_Trsf transform;
    transform.SetScale(gp_Pnt(center[0], center[1], center[2]), scale);
    BRepBuilderAPI_Transform builder {shape.tshape(), transform, true};

    return builder.Shape();
}

Shape scale(const Shape& shape, const vec3& scale)
{
    gp_GTrsf transform;
    transform.SetValue(1,1, scale[0]);
    transform.SetValue(2,2, scale[1]);
    transform.SetValue(3,3, scale[2]);
    BRepBuilderAPI_GTransform builder {shape.tshape(), transform, true};

    return builder.Shape();
}

Shape translate(const Shape& shape, const vec3& dir)
{
    gp_Trsf transform;
    transform.SetTranslation(gp_Vec(dir[0], dir[1], dir[2]));
    BRepBuilderAPI_Transform builder {transform};
    builder.Perform(shape.tshape(), true);

    return builder.Shape();
}

Shape rotate(const Shape& shape, const vec3& pos, const vec3& axis, double angle)
{
    gp_Trsf transform;
    transform.SetRotation(gp_Ax1({pos[0], pos[1], pos[2]}, {axis[0], axis[1], axis[2]}), rad(angle));
    BRepBuilderAPI_Transform builder {shape.tshape(), transform, true};

    return builder.Shape();
}


Compound array(const Shape& shape, const vec3& dir, Size count)
{
    darray<Shape> arr;
    arr.resize(count);
    for (Size n = 0; n < count; ++n)
        arr.push(translate(shape, dir * count));
    return Compound(arr);
}

Compound array(const Shape& shape, const vec3& dir1, Size count1, const vec3& dir2, Size count2)
{
    return array(array(shape, dir1, count1), dir2, count2);
}

Compound array(const Shape& shape, const vec3& dir1, Size count1, const vec3& dir2, Size count2, const vec3& dir3, Size count3)
{
    return array(array(shape, dir1, count1, dir2, count2), dir3, count3);
}

}
