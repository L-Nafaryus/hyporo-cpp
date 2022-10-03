
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

Buffer::~Buffer()
{}

const BufferType Buffer::type() const
{
    return p_type;
}

}