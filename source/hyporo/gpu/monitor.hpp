#pragma once

#include "window_context.hpp"

#include <string>


namespace hpr::gpu
{

class Monitor : public WindowContext
{

protected:

    std::string p_deviceName;
    int p_originX;
    int p_originY;
    int p_width;
    int p_height;
    int p_logicalWidth;
    int p_logicalHeight;

public:

    Monitor();

    Monitor(Provider provider);

    virtual
    ~Monitor();

    // Member functions

    void origin(int x, int y);
    void size(int width, int height);
    void logicalSize(int width, int height);
    void deviceName(const std::string& name);

    inline
    int width() const
    {
        return p_width;
    }

    inline
    int height() const
    {
        return p_height;
    }

    inline
    int logicalWidth() const
    {
        return p_logicalWidth;
    }

    inline
    int logicalHeight() const
    {
        return p_logicalHeight;
    }

    inline
    int originX() const
    {
        return p_originX;
    }

    inline
    int originY() const
    {
        return p_originY;
    }

    inline
    std::string deviceName() const
    {
        return p_deviceName;
    }

};

}