#pragma once

#include "../device.hpp"


namespace hpr::gpu::opengl
{

class Device : public gpu::Device
{

protected:

    bool p_isInitialized;

public:

    // Constructors

    Device();

    ~Device();


};

}