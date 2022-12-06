
#include "buffer.hpp"

#include <glad/glad.h>


namespace hpr::gpu::opengl
{

Buffer::Buffer() :
    gpu::Buffer(DeviceAPI::OpenGL),
    p_bufferIndex {0},
    p_vertexArrayIndex {0}
{}

Buffer::~Buffer() = default;

int Buffer::target() const
{
    switch (p_type)
    {
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;
        case BufferType::Index:
            return GL_ELEMENT_ARRAY_BUFFER;
        default:
            return GL_NONE;
    }
}

}