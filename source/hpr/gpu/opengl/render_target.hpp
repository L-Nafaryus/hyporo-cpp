#pragma once

#include "../render_target.hpp"


namespace hpr::gpu::opengl
{

class RenderTarget : public gpu::RenderTarget
{
    friend class Device;

protected:

    unsigned int p_frameBufferIndex;
    unsigned int p_depthBufferIndex;
    unsigned int p_textureIndex;

public:

    RenderTarget();

    ~RenderTarget() override;

    unsigned int frameBuffer() const
    {
        return p_frameBufferIndex;
    }

    unsigned int depthBuffer() const
    {
        return p_depthBufferIndex;
    }

    unsigned int texture() const
    {
        return p_textureIndex;
    }

};

}