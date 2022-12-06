#pragma once

#include "../containers/array.hpp"
#include "../math/scalar/scalar.hpp"
#include "../math/vector.hpp"



namespace hpr::mesh
{

class Edge;

class Vertex : public vec<scalar, 3>
{
    friend class Mesh;

    using base = vec<scalar, 3>;

protected:
    darray<Edge*> p_refEdges;

public:
    Vertex() :
        base{}
    {}

    Vertex(const scalar& x, const scalar& y, const scalar& z) :
        base{x, y, z}
    {}

    ~Vertex() override
    {
        for (auto& e: p_refEdges)
            e = nullptr;
    }

    darray<Edge*>& refEdges()
    {
        return p_refEdges;
    }

    void addRefEdge(Edge* edge)
    {
        p_refEdges.push(edge);
    }

};

}