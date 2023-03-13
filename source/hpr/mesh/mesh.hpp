#pragma once

#include <hpr/containers/array.hpp>
#include <hpr/math/scalar/scalar.hpp>
#include <hpr/math/vector.hpp>

#include <hpr/mesh/vertex.hpp>
#include <hpr/mesh/edge.hpp>
#include <hpr/mesh/face.hpp>
#include <hpr/mesh/cell.hpp>


namespace hpr::mesh
{

class Mesh
{

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
        for (auto& vertex : edge(n)->vertices())
            vertex->refEdges().remove([this, n](edge_pointer e){ return e == edge(n); });
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
        for (auto& edge : face(n)->edges())
            edge->refFaces().remove([this, n](face_pointer f){ return f == face(n); });
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

    template <std::convertible_to<face_pointer>... Faces>
    void addCell(const Faces& ...faces)
    {
        cells().push(new Cell {static_cast<face_pointer>(faces)...});
        for (auto& face : *cells().back())
            face->addRefCell(cells().back());
    }

    void removeNullCells()
    {
        cells().remove([](cell_pointer cell){ return cell == nullptr; });
    }

    void removeCell(size_type n, bool erase = true, bool cascade = true)
    {
        static_cast<void>(cascade);
        for (auto& face : cell(n)->faces())
            face->refCells().remove([this, n](cell_pointer c){ return c == cell(n); });
        delete cell(n);
        if (erase)
            cells().remove(n);
    }

};

}