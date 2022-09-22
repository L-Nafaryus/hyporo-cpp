#pragma once

#include "Cell.hpp"

namespace hyporo
{

class Mesh
{

    mutable list<shared_ptr<Vertex>> vertices_;
    mutable list<shared_ptr<Edge>> edges_;
    mutable list<shared_ptr<Face>> faces_;
    mutable list<shared_ptr<Cell>> cells_;

public:

    // Constructors

    //- Desctuctor
    virtual ~Mesh();

    // Mesh size parameters

    inline sizet nPoints() const;
    inline sizet nEdges() const;
    inline sizet nFaces() const;
    inline sizet nCells() const;
};

} // end namespace hyporo
