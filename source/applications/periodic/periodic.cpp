#include "../../hpr/math.hpp"
#include "../../hpr/csg.hpp"

#include <iostream>
#include <map>

using namespace hpr;

void print(vec3 vs)
{
    for (auto& v : vs)
        std::cout << v << " ";
    std::cout << std::endl;
}

class Periodic
{
protected:
    scalar p_alpha;
    scalar p_initialRadius;
    scalar p_sideLength;
    scalar p_filletScale;
    vec3 p_direction;

public:
    Periodic() :
        p_alpha {0.1},
        p_initialRadius {1},
        p_filletScale {0.8},
        p_direction {}
    {}

    Periodic(scalar alpha, scalar initialRadius, scalar filletScale, const vec3& direction) :
        p_alpha {alpha},
        p_initialRadius {initialRadius},
        p_filletScale {filletScale},
        p_direction {direction}
    {}

    virtual
    ~Periodic() = default;

    scalar& alpha()
    {
        return p_alpha;
    }

    scalar& initialRadius()
    {
        return p_initialRadius;
    }

    virtual
    scalar sideLength() = 0;

    virtual
    scalar gamma() = 0;

    scalar& filletScale()
    {
        return p_filletScale;
    }

    scalar radius() const
    {
        return p_initialRadius / (1. - p_alpha);
    }

    scalar filletRadius()
    {
        scalar analytical = p_initialRadius * sqrt(2) / sqrt(1 - cos(gamma())) - radius();
        return analytical * p_filletScale;
    }

    vec3& direction()
    {
        return p_direction;
    }

    virtual
    void build() = 0;
};

class Simple : public Periodic, public csg::Shape
{
public:

    Simple() :
        csg::Shape {},
        Periodic {0.01, 1, 0.8, vec3(1., 0., 0.)}
    {}

    Simple(scalar alpha, const vec3& direction, scalar filletScale = 0.8) :
        Simple {}
    {
        p_alpha = alpha;
        p_direction = direction;
        p_filletScale = filletScale;
    }

    Simple(scalar alpha, scalar initialRadius, scalar filletScale, const vec3& direction) :
        Periodic {alpha, initialRadius, filletScale, direction}
    {}

    ~Simple() override = default;

    scalar sideLength() override
    {
        return 2 * initialRadius();
    }

    scalar gamma() override
    {
        return hpr::PI - 2 * 0.5 * 0.5 * hpr::PI;
    }

    csg::Shape lattice()
    {
        csg::Shape lattice;
        darray<csg::Shape> spheres;

        for (int zn = 0; zn < 3; ++zn)
        {
            scalar z = zn * sideLength();
            for (int yn = 0; yn < 3; ++yn)
            {
                scalar y = yn * sideLength();
                for (int xn = 0; xn < 3; ++xn)
                {
                    scalar x = xn * sideLength();
                    spheres.push(csg::sphere(vec3(x, y, z), radius()));
                }
            }
        }

        lattice = csg::fuse({spheres.front()}, spheres.slice(spheres.begin() + 1, spheres.end()));

        if (filletScale() > 0)
        {
            lattice = lattice.scale({0, 0, 0}, 1e+2);
            lattice = lattice.fillet(lattice.edges(), filletRadius() * 1e+2);
            lattice = lattice.scale({0, 0, 0}, 1e-2);
        }

        std::cout << (int)lattice.type() << std::endl;
        return lattice;
    }

    csg::Shape boxCell()
    {
        scalar length = sideLength() * sqrt(2);
        scalar width = sideLength() * sqrt(2);
        scalar height = sideLength();
        scalar xl = sqrt(pow(length, 2) * 0.5);
        scalar yw = xl;
        scalar zh = height;
        darray<Shape> edges {
            csg::Edge({xl, 0, 0}, {0, yw, 0}),
            csg::Edge({0, yw, 0}, {0, yw, zh}),
            csg::Edge({0, yw, zh}, {xl, 0, zh}),
            csg::Edge({xl, 0, zh}, {xl, 0, 0})
        };
        csg::Face plgm {edges};

        vec3 localX {csg::Surface(plgm).normal(0, 0)};
        vec3 localZ = vec3(0, 0, 1);
        vec3 localY = cross(localX, localZ);
        csg::Shape cell = plgm.extrude(localX, width);

        scalar angle;
        hpr::vec3 normal;

        for (auto& face : cell.faces())
        {
            normal = csg::Surface(csg::Face(face)).normal(0, 0);
            angle = hpr::angle(localX, normal);

            if (face.tshape().Orientation() == TopAbs_FORWARD)
            {
                normal = -normal;
                angle = hpr::angle(localX, normal);
            }

            if (equal(angle, 0.))
                face.label("periodic-south");
            else if (equal(angle, hpr::PI))
                face.label("periodic-north");

            angle = hpr::angle(localY, normal);
            if (equal(angle, 0.))
                face.label("periodic-east");
            else if (equal(angle, hpr::PI))
                face.label("periodic-west");

            angle = hpr::angle(localZ, normal);
            if (equal(angle, hpr::PI))
                face.label("periodic-down");
            else if (equal(angle, 0.))
                face.label("periodic-up");
        }

        std::cout << (int)cell.type() << std::endl;
        return cell;
    }

    csg::Shape hexagonalPrismCell()
    {
        return csg::Shape();
    }

    void build() override
    {
        if (direction() == vec3(1., 0., 0.) || direction() == vec3(1., 0., 0.) || direction() == vec3(0., 0., 1.))
            p_shape = csg::cut(boxCell(), lattice()).tshape();
        else if (direction() == vec3(1., 1., 1.))
            p_shape = csg::cut(hexagonalPrismCell(), lattice()).tshape();
        else
            throw std::runtime_error("Undefined cell for passed direction");


        p_shape = this->translate(-this->center()).tshape();
        p_shape = this->rotate(this->center(), {0, 0, 1}, 45).tshape();

        for (auto& face : faces())
            if (face.label() == "default")
                face.label("wall");
    }

};

#include "lattice.hpp"

int main(int argc, char** argv)
{
    /*Simple simple {0.01, {1., 0., 0.}};
    simple.build();

    std::cout << (int)simple.type() << std::endl;
    std::cout << simple.volume() << std::endl;
     */
    csg::Lattice lattice {{2, 2, 2}, {90, 90, 90}, 1, csg::Lattice::Type::Primitive};
    lattice.dump("latticeTest.step", csg::Shape::Format::STEP);
    return 0;
}