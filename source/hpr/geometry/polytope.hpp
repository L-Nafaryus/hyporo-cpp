#pragma once

#include "../containers.hpp"
#include "../math.hpp"


namespace hpr::geometry
{

template <int Dim, int Space>
class Polytope
{

public:

    enum class Type
    {
        Nullitope = -1,
        Monon,
        Dion,
        Polygon,
        Polyhedron,
        Polychoron,
        Unknown
    };

protected:

    const int p_dimension;
    const int p_space;
    Type p_type;
    darray<VectorSpace<scalar, Space>> p_points;

public:

    Polytope() :
        p_dimension {Dim},
        p_space {Space},
        p_type {Type::Unknown},
        p_points {}
    {}

    virtual
    ~Polytope() = default;

};

}