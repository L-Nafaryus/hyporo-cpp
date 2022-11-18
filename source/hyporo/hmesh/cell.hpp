#pragma once

#include "../hyplib/scalar/scalar.hpp"
#include "../hyplib/array/array.hpp"
#include "../hyplib/vector/vector.hpp"


namespace hpr::mesh
{

class Face;

class Cell : public darray<Face*>
{

    using face_pointer = Face*;
    using base = darray<face_pointer>;

public:

    Cell() :
        base {}
    {}

    Cell(std::initializer_list<face_pointer> faces) :
        base{faces}
    {}

    ~Cell() override = default;

    darray<face_pointer>& faces()
    {
        return *this;
    }

};

}