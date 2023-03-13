#pragma once

#include <hpr/containers/array.hpp>
#include <hpr/math/scalar/scalar.hpp>
#include <hpr/math/vector.hpp>



namespace hpr::mesh
{

class Vertex;
class Face;

class Edge : public sarray<Vertex*, 2>
{
    friend class Mesh;

    using vertex_pointer = Vertex*;
    using base = sarray<vertex_pointer, 2>;

protected:
    darray<Face*> p_refFaces;

public:
    Edge() :
        base{}
    {}

    Edge(vertex_pointer v1, vertex_pointer v2) :
        base{v1, v2}
    {}

    ~Edge() override
    {
        for (auto& f: p_refFaces)
            f = nullptr;
    }

    darray<Face*>& refFaces()
    {
        return p_refFaces;
    }

    void addRefFace(Face* face)
    {
        p_refFaces.push(face);
    }

    sarray<vertex_pointer, 2>& vertices()
    {
        return *this;
    }

    vertex_pointer vertex(size_type n)
    {
        return (*this)[n];
    }

    bool isValid()
    {
        return *front() != *back();
    }
};

}