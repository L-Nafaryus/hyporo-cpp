#pragma once

#include "../buffer.hpp"


namespace hpr::gpu::opengl
{

class Buffer : public gpu::Buffer
{
    friend class Device;

protected:
    unsigned int p_bufferIndex;
    unsigned int p_vertexArrayIndex;

public:

    // Constructors

    Buffer();

    virtual ~Buffer();

    // Member functions

    int target() const;
};

}
