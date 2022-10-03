
#include "buffer.hpp"


namespace hpr::gpu::opengl
{

Buffer::Buffer() :
    gpu::Buffer(DeviceAPI::OpenGL),
    p_bufferIndex {0},
    p_vertexArrayIndex {0}
{}

Buffer::~Buffer()
{}

const int Buffer::target() const
{
    switch (p_type)
    {
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;
        case BufferType::Index:
            return GL_ELEMENT_INDEX_BUFFER;
    }
}

}