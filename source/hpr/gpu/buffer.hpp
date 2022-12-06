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
        Uniform,
        BufferTypeCount
    };

protected:

    BufferType p_type;
    int p_size;
    int p_stride;

public:

    // Constructors

    Buffer();

    explicit
    Buffer(DeviceAPI api);

    ~Buffer() override;

    // Member functions

    [[nodiscard]]
    int size() const;

    [[nodiscard]]
    BufferType type() const;
};

} // end namespace hpr::gpu