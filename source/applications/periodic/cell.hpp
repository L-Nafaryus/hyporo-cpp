#pragma once

#include "lattice.hpp"


namespace hpr::csg
{

class Cell //: public csg::Shape
{

protected:

    Lattice p_lattice;

public:

    Cell() = delete;

    Cell(const Lattice& lattice) :
        p_lattice {lattice}
    {}

    Shape operator()(const vec3& size, const vec3& scale = {1, 1, 1}, const vec3& translation = {0, 0, 0}, const vec3& rotation = {0, 0, 0})
    {
        // scale should be equal for both of cell and lattice

        /*Shape box = lattice.box();
        box = box.translate(lattice.os1() * p_shift[0]);
        box = box.translate(lattice.os2() * p_shift[1]);
        box = box.translate(lattice.os3() * p_shift[2]);*/
        Shape cellBox = csg::box({0, 0, 0}, scale);
        vec3 dir = -cellBox.center();
        cellBox = csg::translate(cellBox, dir);
        cellBox = csg::rotate(cellBox, cellBox.center(), {1, 0, 0}, rotation[0]);
        cellBox = csg::rotate(cellBox, cellBox.center(), {0, 1, 0}, rotation[1]);
        cellBox = csg::rotate(cellBox, cellBox.center(), {0, 0, 1}, rotation[2]);
        cellBox = csg::translate(cellBox, translation);
        cellBox = csg::scale(cellBox, size);

        Shape lattice = p_lattice(size, {90, 90, 90}, 1.1);
        lattice = csg::translate(lattice, -size * 0.5);

        return csg::cut(cellBox, lattice).tshape();
    }
};

}