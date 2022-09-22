#pragma once

/** \file
 * \ingroup hyplib
 **/

#include "integer.hpp"
#include "scalar.hpp"
#include <array>
#include <ostream>

namespace hyporo
{

// Forward declaration of friend functions and operators

template <class C, sizet NC> class VectorSpace;

template <class C, sizet NC>
std::ostream& operator<<(std::ostream&, const VectorSpace<C, NC>&);

// Class declaration

template <class C, sizet NC>
class VectorSpace
{

public:

    //- The components of this vector space
    std::array<C, NC> row;

    // Static constants

    static const sizet nComponents = NC;

    // Constructors

    inline VectorSpace();

    //inline VectorSpace(const VectorSpace<C, NC>&);

    template <class... Args>
    inline VectorSpace(const Args... components);


    // Member functions

    inline static sizet size();

    // Member operators

    inline C& operator[](const sizet);

    inline VectorSpace<C, NC>& operator*=(const scalar);

    inline VectorSpace<C, NC>& operator/=(const scalar);

    // Friend operators

    friend std::ostream& operator<< <C, NC>(std::ostream&, const VectorSpace<C, NC>&);
};

} // end namespace hyporo

#include "VectorSpace.hxx"
