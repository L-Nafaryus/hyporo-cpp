#pragma once

#include "../../hyporo/hyplib/scalar/scalar.hpp"
#include "../../hyporo/hyplib/vector/vector.hpp"
#include "../../hyporo/hyplib/array/array.hpp"
#include "../../hyporo/hyplib/matrix/matrix.hpp"
#include "../../hyporo/csg/csg.hpp"


namespace hpr::csg
{
void prints(scalar point)
{
    std::cout << point << std::endl;
}
class Lattice : public csg::Shape
{

public:

    enum class System
    {
        Triclinic,
        Monoclinic,
        Orthorhombic,
        Tetragonal,
        Rhombohedral,
        Hexagonal,
        Cubic,
        Unknown
    };

    enum class Type
    {
        Primitive,
        BaseCentered,
        BodyCentered,
        FaceCentered,
        Unknown
    };

protected:

    vec3 p_lengths;
    vec3 p_angles;
    Type p_type;
    scalar p_radius;
    darray<vec3> p_controlPoints;

public:

    Lattice() = delete;

    Lattice(const vec3& lengths, const vec3& angles, scalar radius, Type type) :
        csg::Shape {},
        p_lengths {lengths},
        p_angles {angles},
        p_radius {radius},
        p_type {type}
    {
        generateControlPoints();
        darray<csg::Shape> spheres;
        for (const auto& point : controlPoints()) {
            spheres.push(csg::sphere(point, p_radius));
            print(point);
        }

        p_shape = csg::Compound(spheres).tshape();//csg::fuse({spheres.front()}, spheres.slice(spheres.begin() + 1, spheres.end())).tshape();
    }

    darray<vec3> controlPoints() const
    {
        return p_controlPoints;
    }

    vec3 lengths() const
    {
        return p_lengths;
    }

    vec3 angles() const
    {
        return p_angles;
    }

    void generateControlPoints()
    {
        if (p_type == Type::Unknown)
            throw std::runtime_error("Unknown type of lattice");
        p_controlPoints.resize(14);
        //
        vec3 ox {1, 0, 0};
        vec3 oy {0, 1, 0};
        vec3 oz {0, 0, 1};
        vec3 ox1 = hpr::rotate(ox, oz, radians(-p_angles[2]));
        p_controlPoints.push(vec3{0, 0, 0});
        p_controlPoints.push(vec3{0, p_lengths[0], 0});
        vec3 t1 = hpr::translate(p_controlPoints.back(), ox1 * p_lengths[1]);
        p_controlPoints.push(t1);
        p_controlPoints.push(hpr::translate(p_controlPoints.front(), ox1 * p_lengths[1]));
        print(t1);
        print(ox1);
        scalar c1 = cos(radians(p_angles[2])), c2 = cos(radians(p_angles[1])), c3 = cos(radians(p_angles[0]));
        scalar D1 = sqrt(mat3(
            1, cos(radians(p_angles[2])), cos(radians(p_angles[1])),
            cos(radians(p_angles[2])), 1, cos(radians(p_angles[0])),
            cos(radians(p_angles[1])), cos(radians(p_angles[0])), 1).det());
        scalar volume = 1. / 6. * p_lengths[0] * p_lengths[1] * p_lengths[2] *
            D1;
        scalar s1 = sqrt(std::pow(p_lengths[0], 2) + std::pow(p_lengths[1], 2) - 2 *
            p_lengths[0] * p_lengths[1] * cos(radians(p_angles[2])));
        scalar s2 = sqrt(std::pow(p_lengths[1], 2) + std::pow(p_lengths[2], 2) - 2 *
            p_lengths[1] * p_lengths[2] * cos(radians(p_angles[1])));
        scalar s3 = sqrt(std::pow(p_lengths[0], 2) + std::pow(p_lengths[2], 2) - 2 *
            p_lengths[0] * p_lengths[2] * cos(radians(p_angles[0])));
        scalar area = 1. / 2. * p_lengths[0] * p_lengths[1] *
            sqrt(mat2{1, cos(radians(p_angles[2])), cos(radians(p_angles[2])), 1}.det());
        scalar h1 = 3 * volume / area;
        scalar a1 = asin(h1 / p_lengths[2]);
        scalar sh1 = sqrt(std::pow(p_lengths[2], 2) - std::pow(h1, 2));
        scalar sh2 = p_lengths[2] * cos(radians(p_angles[0]));
        scalar a2 = acos(sh2 / sh1);

        vec3 ox2 = hpr::rotate(ox, oy, a1);
        if (!std::isnan(a2))
            ox2 = hpr::rotate(ox2, oz, a2);
        print(ox2);
        for (auto n = 0; n < 4; ++n)
            p_controlPoints.push(hpr::translate(p_controlPoints[n], ox2 * p_lengths[2]));

        /*p_controlPoints.push(vec3{p_lengths[0], p_lengths[1], 0});
        p_controlPoints.push(vec3{p_lengths[0], 0, 0});

        p_controlPoints.push(vec3{0, 0, p_lengths[2]});
        p_controlPoints.push(vec3{0, p_lengths[1], p_lengths[2]});
        p_controlPoints.push(vec3{p_lengths[0], p_lengths[1], p_lengths[2]});
        p_controlPoints.push(vec3{p_lengths[0], 0, p_lengths[2]});

        // central points on base faces
        if (p_type == Type::BaseCentered || p_type == Type::FaceCentered)
        {
            for (int n = 0; n < 2; ++n)
            {
                vec3 center;
                for (int k = 0; k < 4; ++k)
                    center += p_controlPoints[k + 4 * n];
                p_controlPoints.push(center * 0.25);
            }
        }

        // central point (center of mass)
        if (p_type == Type::BodyCentered)
        {
            vec3 center;
            for (const auto& point : p_controlPoints)
                center += point;
            p_controlPoints.push(center / p_controlPoints.size());
        }

        // central points on side faces
        if (p_type == Type::FaceCentered)
        {
            for (int n = 0; n < 3; ++n)
            {
                vec3 center;
                for (int k = 0; k < 2; ++k)
                {
                    center += p_controlPoints[n + k];
                    center += p_controlPoints[n + k + 4];
                }
                p_controlPoints.push(center * 0.25);
            }
            vec3 center;
            for (int n = 0; n < 2; ++n)
            {
                center += p_controlPoints[n * 3];
                center += p_controlPoints[4 + n * 3];
            }
            p_controlPoints.push(center * 0.25);
        }

        mat4 trans = mat4::identity();
        vec3 ox {1, 0, 0};
        vec3 oy {0, 1, 0};
        vec3 oz {0, 0, 1};
        int n = 0;
        for (auto& point : p_controlPoints)
        {
            if (n == 0 || n == 3)
            {
                ++n;
                continue;
            }
            trans.row(3, vec4(point, 0));
            trans = hpr::rotate(trans, oz, -radians(90 - p_angles[2]));
            if (n >= 4 && n <= 7)
            {
                trans = hpr::rotate(trans, ox, -radians(90 - p_angles[1]));
                trans = hpr::rotate(trans, oy, -radians(90 - p_angles[0]));
            }
            point = vec3(trans.row(3)[0], trans.row(3)[1], trans.row(3)[2]);
            ++n;
        }*/

    }
};

}