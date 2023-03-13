#pragma once

#include <hpr/math/vector.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif


namespace hpr::gpu
{

class Viewport
{

protected:

    vec2 p_pos;
    vec2 p_size;

public:

    inline
    Viewport() :
        p_pos {0.f, 0.f},
        p_size {0.f, 0.f}
    {}

    inline
    Viewport(const vec2& pos, const vec2& size) :
        p_pos {pos},
        p_size {size}
    {}

    virtual
    ~Viewport() = default;

    vec2& pos()
    {
        return p_pos;
    }

    vec2& size()
    {
        return p_size;
    }

    inline
    void set()
    {
        glViewport(p_pos[0], p_pos[1], p_size[0], p_size[1]);
    }

};

}