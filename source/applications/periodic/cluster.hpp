#pragma once

#include "cell.hpp"


namespace hpr::csg
{

    class Cluster //: public csg::Shape
    {

    protected:

        vec3 p_cluster;

    public:

        Cluster() = delete;

        Cluster(vec3 cluster, const Lattice& lattice, const vec3& shift) :
            p_cluster {cluster}
        {}

        Shape operator()()
        {
            auto cell = Cell(lattice, shift);
            darray<Shape> args {cell};
            darray<Shape> xcells {cell};
            darray<Shape> ycells ;
            darray<Shape> zcells ;

            for (auto n = 0; n <= cluster[0]; ++n)
                xcells.push(cell.translate(lattice.os1() * lattice.lengths()[0] * n));

            for (auto n = 0; n <= cluster[1]; ++n)
                for (const auto& cell_ : xcells)
                    ycells.push(cell_.translate(lattice.os2() * lattice.lengths()[1] * n));

            for (auto n = 0; n <= cluster[2]; ++n)
                for (const auto& cell_ : ycells)
                    zcells.push(cell_.translate(lattice.os3() * lattice.lengths()[2] * n));

            p_shape = csg::fuse({zcells.pop()}, zcells).tshape();
        }
    };

}
