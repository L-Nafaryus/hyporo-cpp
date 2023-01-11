#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif


namespace hpr::gpu
{

class DepthBuffer
{

protected:

    bool p_enabled;
    bool p_binded;

public:

    inline
    DepthBuffer() :
        p_enabled {true},
        p_binded {false}
    {}

    inline
    DepthBuffer(bool enabled) :
        p_enabled {enabled},
        p_binded {false}
    {}

    virtual
    ~DepthBuffer() = default;

    inline
    void bind()
    {
        p_binded = true;
        glEnable(GL_DEPTH_TEST);
    }

    inline
    void unbind()
    {
        p_binded = false;
        glDisable(GL_DEPTH_TEST);
    }

    inline
    bool binded() const
    {
        return p_binded;
    }

    inline
    void enable()
    {
        p_enabled = true;
        glDepthMask(GL_TRUE);
    }

    inline
    void disable()
    {
        p_enabled = false;
        glDepthMask(GL_FALSE);
    }

    [[nodiscard]]
    inline
    bool enabled() const
    {
        return p_enabled;
    }

    inline
    void clear() const
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
};

}
