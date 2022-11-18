
#include "buffer.hpp"


namespace hpr::gpu
{

Buffer::Buffer() :
    Context {DeviceAPI::Unknown},
    p_type {BufferType::Undefined},
    p_size {0},
    p_stride {0}
{}

Buffer::Buffer(DeviceAPI api) :
    Context {api},
    p_type {BufferType::Undefined},
    p_size {0},
    p_stride {0}
{}

Buffer::~Buffer() = default;

// Member functions

int Buffer::size() const
{
    return p_size;
}

Buffer::BufferType Buffer::type() const
{
    return p_type;
}

}