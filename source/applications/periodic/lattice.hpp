#pragma once

#include "../../hpr/math.hpp"
#include "../../hpr/csg.hpp"


namespace hpr::csg
{
void prints(scalar point)
{
    std::cout << point << std::endl;
}
class Lattice //: public csg::Shape
{

public:

    enum LatticeSystem
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

    enum LatticeType
    {
        Primitive,
        BaseCentered,
        BodyCentered,
        FaceCentered
    };

protected:

    vec3 p_lengths;
    vec3 p_angles;
    LatticeType p_type;
    scalar p_radius;
    Vector<unsigned int, 3> p_cluster;
    darray<vec3> p_cornerPoints;
    darray<vec3> p_faceBodyPoints;

public:

    Lattice() = delete;

    Lattice(LatticeType ltype) : //const vec3& lengths, const vec3& angles, scalar radius, LatticeType ltype) :
        //csg::Shape {},
        p_lengths {},//{lengths},
        p_angles {},//{angles},
        p_radius {},//{radius},
        p_type {ltype}
    {}

    darray<vec3> cornerPoints() const
    {
        return p_cornerPoints;
    }

    darray<vec3> faceBodyPoints() const
    {
        return p_faceBodyPoints;
    }

    vec3 lengths() const
    {
        return p_lengths;
    }

    vec3 angles() const
    {
        return p_angles;
    }

    darray<scalar> faceIndices() const
    {
        return {
            2, 6, 4, 0,     // x0
            1, 5, 7, 3,     // x+ 1
            0, 4, 5, 1,     // y0
            3, 7, 6, 2,     // y+ 3
            2, 0, 1, 3,     // z0
            4, 6, 7, 5      // z+ 5
        };
    }

    inline
    vec3 os1() const
    {
        return {1, 0, 0};
    }

    inline
    vec3 os2() const
    {
        return hpr::rotate(os1(), {0, 0, 1}, rad(p_angles[2]));
    }

    inline
    vec3 os3() const
    {
        vec3 oz {0, 0, 1};
        vec3 os3_ = hpr::rotate(os1(), oz, rad(0.5 * p_angles[2]));
        scalar gamma = asin(sqrt(1 - 3 * pow(cos(rad(p_angles[0])), 2) + 2 * pow(cos(rad(p_angles[0])), 3)) / sin(rad(p_angles[0])));

        return hpr::rotate(os3_, hpr::rotate(os3_, oz, rad(90)), -gamma);
    }

    void generateControlPoints(const vec3& lengths, const vec3& angles)
    {
        p_angles = angles;
        p_lengths = lengths;

        auto translatePoint = [](const hpr::vec3& v1, const hpr::vec3& v2)
        {
            return hpr::vec3(hpr::translate(hpr::mat4::identity(), v2) * hpr::vec4(v1, 1.0f));
        };

        if (!p_cornerPoints.is_empty())
            p_cornerPoints.clear();
        if (!p_faceBodyPoints.is_empty())
            p_faceBodyPoints.clear();

        p_cornerPoints.resize(8);
        p_faceBodyPoints.resize(7);

        // p_corner points
        p_cornerPoints.push(vec3{0, 0, 0});
        p_cornerPoints.push(vec3(lengths[0], 0, 0));
        p_cornerPoints.push(translatePoint(p_cornerPoints[0], os2() * lengths[1]));
        p_cornerPoints.push(translatePoint(p_cornerPoints[1], os2() * lengths[1]));

        for (auto n = 0; n < 4; ++n)
            p_cornerPoints.push(translatePoint(p_cornerPoints[n], os3() * lengths[2]));

        // face/body points
        // central points on base faces
        if (p_type == BaseCentered || p_type == FaceCentered)
        {
            for (int n = 0; n < 2; ++n)
            {
                vec3 center;
                for (int k = 0; k < 4; ++k)
                    center += p_cornerPoints[static_cast<Size>(faceIndices()[k + 4 * (n + 4)])];
                p_faceBodyPoints.push(center * 0.25);
            }
        }

        // central point (center of mass)
        if (p_type == BodyCentered)
        {
            p_faceBodyPoints.push(sum(p_cornerPoints) / p_cornerPoints.size());
        }

        // central points on side faces
        if (p_type == FaceCentered)
        {
            for (int n = 0; n < 4; ++n)
            {
                vec3 center;
                for (int k = 0; k < 4; ++k)
                    center += p_cornerPoints[static_cast<Size>(faceIndices()[k + n * 4])];
                p_faceBodyPoints.push(center * 0.25);
            }
        }
    }

    Shape operator()(const vec3& lengths, const vec3& angles, scalar radius)
    {
        generateControlPoints(lengths, angles);

        darray<csg::Shape> args;
        darray<csg::Shape> tools;
        bool skip = false;

        for (const auto& point : cornerPoints())
        {
            if (!skip) {
                args.push(csg::sphere(point, radius));
                skip = true;
            }
            else
                tools.push(csg::sphere(point, radius));
        }

        for (const auto& point : faceBodyPoints()) {
            tools.push(csg::sphere(point, radius));
        }

        return csg::fuse(args, tools).tshape();
        //args.push(tools);
        //return Compound(args);
    }

    Shape box() const
    {
        darray<Face> faces;

        for (auto n = 0; n < 6; ++n)
        {
            darray<Edge> edges;

            edges.push(Edge(cornerPoints()[faceIndices()[0 + n * 4]], cornerPoints()[faceIndices()[1 + n * 4]]));
            edges.push(Edge(cornerPoints()[faceIndices()[1 + n * 4]], cornerPoints()[faceIndices()[2 + n * 4]]));
            edges.push(Edge(cornerPoints()[faceIndices()[2 + n * 4]], cornerPoints()[faceIndices()[3 + n * 4]]));
            edges.push(Edge(cornerPoints()[faceIndices()[3 + n * 4]], cornerPoints()[faceIndices()[0 + n * 4]]));
            faces.push(Face(edges));
        }

        return Solid(Shell::sew(faces));
    }
};

}