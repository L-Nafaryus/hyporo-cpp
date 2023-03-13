#pragma once

#include <hpr/containers/array.hpp>
#include <hpr/math/scalar/scalar.hpp>
#include <hpr/math/vector.hpp>



namespace hpr::mesh
{

class Vertex;
class Edge;
class Cell;

class Face : public darray<Edge*>
{
    friend class Mesh;

    using edge_pointer = Edge*;
    using vertex_pointer = Vertex*;
    using base = darray<edge_pointer>;

protected:
    darray<Cell*> p_refCells;

public:
    Face() :
        base{}
    {}

    Face(std::initializer_list<edge_pointer> edges) :
        base{edges}
    {}

    ~Face() override
    {
        for (auto& c: p_refCells)
            c = nullptr;
    }

    darray<Cell*>& refCells()
    {
        return p_refCells;
    }

    void addRefCell(Cell* cell)
    {
        p_refCells.push(cell);
    }

    darray<edge_pointer>& edges()
    {
        return *this;
    }

    edge_pointer edge(size_type n)
    {
        return (*this)[n];
    }

    darray<vertex_pointer> vertices()
    {
        darray<vertex_pointer> vertices_ {size(), nullptr};
        for (auto n = 0; n < size(); ++n)
            vertices_[n] = edge(n)->vertex(0);
        return vertices_;
    }

};

}