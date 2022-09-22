#pragma once

#include "vector.hpp"
#include <memory>
#include <vector>

namespace hyporo
{

template <class T>
using list = std::vector<T>;

using std::shared_ptr;
using std::weak_ptr;

// Forward declaration

class Edge;

class Mesh;

// Class declaration

class Vertex : public VectorSpace<scalar, 3>
{

    //- List of weak pointers to edges that use this vertex
    mutable list<weak_ptr<Edge>> edges_;

public:

    // Constructors

    inline Vertex(Mesh& mesh, const scalar& x, const scalar& y, const scalar& z);

    // Member functions

    inline const scalar& x() const;

    inline const scalar& y() const;

    inline const scalar& z() const;
};

} // end namespace hyporo

#include "Vertex.hxx"
