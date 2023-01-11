#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif



namespace hpr::gpu
{

class CullFace
{

    enum class Mode
    {
        Front = GL_FRONT,
        Back = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK,
        None = GL_NONE
    };

protected:

    bool p_binded;
    Mode p_mode;

public:

    inline
    CullFace() :
        p_binded {false},
        p_mode {Mode::FrontAndBack}
    {}

    inline
    CullFace(Mode mode) :
        p_binded {false},
        p_mode {mode}
    {}

    virtual
    ~CullFace() = default;

    inline
    void bind()
    {
        p_binded = true;
        glEnable(GL_CULL_FACE);
    }

    inline
    void unbind()
    {
        p_binded = false;
        glDisable(GL_CULL_FACE);

    }

    inline
    bool binded() const
    {
        return p_binded;
    }

    inline
    void set(Mode mode)
    {
        p_mode = mode;
        glCullFace(static_cast<GLenum>(mode));
    }

};

}

