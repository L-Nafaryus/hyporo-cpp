#pragma once

#include "../hyplib/scalar/scalar.hpp"
#include "../hyplib/array/array.hpp"
#include "../hyplib/vector/vector.hpp"

#include <memory>

#include <vector>
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
            base {}
    {}
    Vertex(const scalar& x, const scalar& y, const scalar& z) :
            base {x, y, z}
    {}
    virtual
    ~Vertex()
    {
        for (auto& e : p_refEdges)
            delete e;
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
            base {}
    {}
    Edge(vertex_pointer v1, vertex_pointer v2) :
            base {v1, v2}
    {}
    virtual
    ~Edge()
    {
        for (auto& f : p_refFaces)
            delete f;
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
};
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
            base {}
    {}
    Face(std::initializer_list<edge_pointer> edges) :
            base {edges}
    {}
    virtual
    ~Face()
    {
        for (auto& c : p_refCells)
            delete c;
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

class Cell : public darray<Face*>
{
    using face_pointer = Face*;
    using base = darray<face_pointer>;
public:
    Cell(std::initializer_list<face_pointer> faces) :
            base {faces}
    {}
};


class Mesh
{
#include "vertex.hpp"
public:
    using size_type = std::size_t;
    using vertex_pointer = Vertex*;
    using edge_pointer = Edge*;
    using face_pointer = Face*;
    using cell_pointer = Cell*;
protected:
    darray<Vertex*> p_vertices;
    darray<Edge*> p_edges;
    darray<Face*> p_faces;
    darray<Cell*> p_cells;
public:

    Mesh() = default;
    Mesh(const Mesh&) = default;
    ~Mesh()
    {
        for (auto& v : p_vertices)
            delete v;
        for (auto& e : p_edges)
            delete e;
        for (auto& f : p_faces)
            delete f;
        for (auto& c : p_cells)
            delete c;
    }

    int indexOf(vertex_pointer v)
    {
        for (int n = 0; n < vertices().size(); ++n)
            if (vertex(n) == v)
                return n;
        return -1;
    }

    int indexOf(edge_pointer e)
    {
        for (int n = 0; n < edges().size(); ++n)
            if (edge(n) == e)
                return n;
        return -1;
    }

    int indexOf(face_pointer e)
    {
        for (int n = 0; n < faces().size(); ++n)
            if (face(n) == e)
                return n;
        return -1;
    }

    int indexOf(cell_pointer e)
    {
        for (int n = 0; n < cells().size(); ++n)
            if (cell(n) == e)
                return n;
        return -1;
    }

    darray<Vertex*>& vertices()
    {
        return p_vertices;
    }

    [[nodiscard]]
    vertex_pointer vertex(size_type n) const
    {
        return p_vertices[n];
    }

    void addVertex(const scalar& x, const scalar& y, const scalar& z)
    {
        p_vertices.push(new Vertex(x, y, z));
    }

    void removeNullVertices()
    {
        vertices().remove([](vertex_pointer vertex){ return vertex == nullptr; });
    }

    void removeVertex(size_type n, bool erase = true, bool cascade = true)
    {
        if (cascade)
        {
            for (auto &refEdge: vertex(n)->refEdges())
                removeEdge(indexOf(refEdge), false);
            removeNullEdges();
        }
        delete vertex(n);
        if (erase)
            vertices().remove(n);
    }

    darray<Edge*>& edges()
    {
        return p_edges;
    }

    [[nodiscard]]
    edge_pointer edge(size_type n) const
    {
        return p_edges[n];
    }

    void addEdge(vertex_pointer v1, vertex_pointer v2)
    {
        edges().push(new Edge {v1, v2});
        v1->addRefEdge(edges().back());
        v2->addRefEdge(edges().back());
    }

    void removeNullEdges()
    {
        edges().remove([](edge_pointer edge){ return edge == nullptr; });
    }

    void removeEdge(size_type n, bool erase = true, bool cascade = true)
    {
        if (cascade)
        {
            for (auto &refFace: edge(n)->refFaces())
                removeFace(indexOf(refFace), false);
            removeNullFaces();
        }
        delete edge(n);
        if (erase)
            edges().remove(n);
    }
    darray<Face*>& faces()
    {
        return p_faces;
    }

    [[nodiscard]]
    face_pointer face(size_type n) const
    {
        return p_faces[n];
    }

    template <std::convertible_to<edge_pointer>... Edges>
    void addFace(const Edges& ...edges)
    {
        faces().push(new Face {static_cast<edge_pointer>(edges)...});
        for (auto& edge : *faces().back())
            edge->addRefFace(faces().back());
    }

    void removeNullFaces()
    {
        faces().remove([](face_pointer face){ return face == nullptr; });
    }

    void removeFace(size_type n, bool erase = true, bool cascade = true)
    {
        if (cascade)
        {
            for (auto &refCell: face(n)->refCells())
                removeCell(indexOf(refCell), false);
            removeNullFaces();
        }
        delete face(n);
        if (erase)
            faces().remove(n);
    }

    darray<Cell*>& cells()
    {
        return p_cells;
    }

    [[nodiscard]]
    cell_pointer cell(size_type n) const
    {
        return p_cells[n];
    }

    void removeNullCells()
    {
        cells().remove([](cell_pointer cell){ return cell == nullptr; });
    }

    void removeCell(size_type n, bool erase = true, bool cascade = true)
    {
        static_cast<void>(cascade);
        delete cell(n);
        if (erase)
            cells().remove(n);
    }
};

}