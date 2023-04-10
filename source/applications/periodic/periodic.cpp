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

#include "lattice.hpp"
#include "cell.hpp"
//#include "cluster.hpp"

int main(int argc, char** argv)
{
    /*Simple simple {0.01, {1., 0., 0.}};
    simple.build();

    std::cout << (int)simple.type() << std::endl;
    std::cout << simple.volume() << std::endl;
     */
    auto translatePoint = [](const hpr::vec3& v1, const hpr::vec3& v2)
    {
        return hpr::vec3(hpr::translate(hpr::mat4::identity(), v2) * hpr::vec4(v1, 1.0f));
    };
    csg::Lattice lattice {csg::Lattice::Primitive};//{{2, 2, 2}, {90, 90, 90}, 1.1, csg::Lattice::Primitive};
    //lattice.dump("latticeTest.step", csg::Shape::Format::STEP);
    //lattice.box().dump("latticeBoxTest.step", csg::Shape::Format::STEP);
    csg::Shape cell = csg::Cell(lattice)({2, 2, 2}, {1.414213, 1.414213, 1}, {1.41421, 1.41421, 0}, {0, 0, 45});
    //std::cout << (int)cell.type() << std::endl;
    //csg::Cluster cluster {{2, 2, 2}, lattice, {0, 0, 0}};
    cell.dump("cellTest.step", csg::Shape::Format::STEP);

    //darray<csg::Shape> clusterShapes = csg::array(cell, lattice.os1(), 3, lattice.os2(), 3, lattice.os3(), 3).subShapes(csg::Shape::Type::Solid);
    //std::cout << clusterShapes.size() << std::endl;
    //csg::Shape cluster = csg::fuse({clusterShapes.pop()}, clusterShapes);
    //cluster.dump("clusterTest.step", csg::Shape::Format::STEP);


    /*csg::Lattice lattice2 {{4 / sqrt(3), 4 / sqrt(3), 4 / sqrt(3)}, {90, 90, 90}, 1.1, csg::Lattice::BodyCentered};
    csg::Cluster cluster2 {{2, 2, 2}, lattice2, {0, 0, 0}};
    csg::Lattice lattice3 {{4 / sqrt(2), 4 / sqrt(2), 4 / sqrt(2)}, {90, 90, 90}, 1.1, csg::Lattice::FaceCentered};
    csg::Cluster cluster3 {{2, 2, 2}, lattice3, {0, 0, 0}};
    cluster3.dump("cluster3Test.step", csg::Shape::Format::STEP);*/
    return 0;
}