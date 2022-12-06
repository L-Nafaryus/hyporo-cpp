#pragma once

#include "context.hpp"


namespace hpr::gpu
{

class RenderTarget : public Context
{
    friend class Device;

public:

    enum class Type
    {
        Unknown,
        Screen,
        Framebuffer
    };

protected:

    int p_posX;
    int p_posY;
    int p_width;
    int p_height;

    Type p_type;

    bool p_hasColorData;
    bool p_hasDepthBuffer;
    bool p_depthTestEnabled;

    RenderTarget* p_parent;

public:

    RenderTarget();

    RenderTarget(DeviceAPI api);

    virtual
    ~RenderTarget();

    int posX() const
    {
        return p_posX;
    }

    int posY() const
    {
        return p_posY;
    }

    int width() const
    {
        return p_width;
    }

    int height() const
    {
        return p_height;
    }

    Type type() const
    {
        return p_type;
    }

    bool hasColorData() const
    {
        return p_hasColorData;
    }

    bool hasDepthBuffer() const
    {
        return p_hasDepthBuffer;
    }

    bool isDepthTestEnabled() const
    {
        return p_depthTestEnabled;
    }

    void depthTest(bool enable)
    {
        p_depthTestEnabled = enable;
    }

    RenderTarget* parent()
    {
        return p_parent;
    }

};

}