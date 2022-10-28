#include "monitor.hpp"


namespace hpr::gpu
{

Monitor::Monitor() :
    WindowContext(Provider::Unknown),
    p_deviceName {},
    p_originX {0},
    p_originY {0},
    p_width {0},
    p_height {0},
    p_logicalWidth {0},
    p_logicalHeight {0}
{}

Monitor::Monitor(Provider provider) :
        WindowContext(provider),
        p_deviceName {},
        p_originX {0},
        p_originY {0},
        p_width {0},
        p_height {0},
        p_logicalWidth {0},
        p_logicalHeight {0}
{}

Monitor::~Monitor() = default;

void Monitor::origin(int x, int y)
{
    p_originX = x;
    p_originY = y;
}

void Monitor::size(int width, int height)
{
    p_width = width;
    p_height = height;
}

void Monitor::logicalSize(int width, int height)
{
    p_logicalWidth = width;
    p_logicalHeight = height;
}

void Monitor::deviceName(const std::string &name)
{
    p_deviceName = name;
}


}