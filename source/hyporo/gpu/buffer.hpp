#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Buffer : public Context
{
    friend class Device;

public:

    enum class BufferType
    {
        Undefined,
        Vertex,
        Index,
        BufferTypeCount
    };

protected:

    BufferType p_type;
    int p_size;
    int p_stride;

public:

    // Constructors

    Buffer();

    Buffer(DeviceAPI api);

    virtual ~Buffer();

    // Member functions

    int size() const;

    BufferType type() const;
};

} // end namespace hpr::gpu