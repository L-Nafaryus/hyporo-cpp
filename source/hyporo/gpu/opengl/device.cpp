
#include "device.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture.hpp"

#include "io/io.hpp"

#include <glad/glad.h>
#include <stdexcept>


namespace hpr::gpu::opengl
{

Device::Device() :
    gpu::Device {DeviceAPI::OpenGL},
    p_isInitialized {false}
{}

Device::~Device()
{}

// Setup

bool Device::initialize()
{
    return p_isInitialized = true;
}

bool Device::destroy()
{
    return p_isInitialized = false;
}

// State

void Device::faceCulling(bool enableFaceCulling, CullMode faceCullingMode)
{
    if (enableFaceCulling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    switch (faceCullingMode)
    {
        case CullMode::Front:
            glCullFace(GL_FRONT);
            break;
        case CullMode::Back:
            glCullFace(GL_BACK);
            break;
        case CullMode::FrontAndBack:
            glCullFace(GL_FRONT_AND_BACK);
            break;
        case CullMode::None:
            glCullFace(GL_NONE);
            break;
    }
}

void Device::createVertexBuffer(gpu::Buffer **buffer, int size, char *data)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *buffer = nullptr;
    p_buffers.emplace_back(opengl::Buffer());
    opengl::Buffer* newBuffer = dynamic_cast<opengl::Buffer*>(&p_buffers.back());

    newBuffer->p_type = Buffer::BufferType::Vertex;
    newBuffer->p_size = size;

    glGenVertexArrays(1, &newBuffer->p_vertexArrayIndex);
    glBindVertexArray(newBuffer->p_vertexArrayIndex);

    glGenBuffers(1, &newBuffer->p_bufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, newBuffer->p_bufferIndex);
    glBufferData(GL_ARRAY_BUFFER, size, (void*)data, GL_STATIC_DRAW);

    *buffer = static_cast<gpu::Buffer*>(newBuffer);
}

void Device::createIndexBuffer(gpu::Buffer **buffer, int size, char *data)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *buffer = nullptr;
    p_buffers.emplace_back(opengl::Buffer());
    opengl::Buffer* newBuffer = dynamic_cast<opengl::Buffer*>(&p_buffers.back());

    newBuffer->p_type = Buffer::BufferType::Vertex;
    newBuffer->p_size = size;

    glGenVertexArrays(1, &newBuffer->p_vertexArrayIndex);
    glBindVertexArray(newBuffer->p_vertexArrayIndex);

    glGenBuffers(1, &newBuffer->p_bufferIndex);
    glBindBuffer(GL_UNIFORM_BUFFER, newBuffer->p_bufferIndex);
    glBufferData(GL_UNIFORM_BUFFER, size, (void*)data, GL_STATIC_DRAW);

    *buffer = static_cast<gpu::Buffer*>(newBuffer);
}

void Device::useVertexBuffer(gpu::Buffer *buffer, int stride, int offset)
{
    if (buffer == nullptr)
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else
    {
        opengl::Buffer* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);

        if (curBuffer->p_type == Buffer::BufferType::Vertex && p_currentVertexBuffer != buffer)
        {
            glBindVertexArray(curBuffer->p_vertexArrayIndex);
            glBindBuffer(GL_ARRAY_BUFFER, curBuffer->p_bufferIndex);

            opengl::Buffer* curIndexBuffer = dynamic_cast<opengl::Buffer*>(p_currentIndexBuffer);

            if (curIndexBuffer != nullptr)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curIndexBuffer->p_bufferIndex);
        }
    }
    gpu::Device::useVertexBuffer(buffer, stride, offset);
}

void Device::useIndexBuffer(gpu::Buffer *buffer, int offset)
{
    if (buffer == nullptr)
    {
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        opengl::Buffer* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);

        if (curBuffer->p_type == Buffer::BufferType::Index && p_currentVertexBuffer != buffer)
        {
            glBindVertexArray(curBuffer->p_vertexArrayIndex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curBuffer->p_bufferIndex);
        }
    }
    gpu::Device::useIndexBuffer(buffer, offset);
}

void Device::destroyBuffer(gpu::Buffer *&buffer)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    opengl::Buffer* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);
    glDeleteBuffers(1, &curBuffer->p_bufferIndex);

    if (curBuffer->p_type == Buffer::BufferType::Vertex)
        glDeleteVertexArrays(1, &curBuffer->p_vertexArrayIndex);

    gpu::Device::destroyBuffer(buffer);
}

// Shaders

void Device::createVertexShader(gpu::Shader **shader, const std::string &filename, const std::string &label)
{
    if (shader == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *shader = nullptr;

    unsigned int shaderIndex = glCreateShader(GL_VERTEX_SHADER);
    if (shaderIndex == 0)
        throw std::runtime_error("Could not create shader");

    File file;
    file.open(filename, File::Binary | File::Read);
    std::string content = file.read().str();
    const char* shaderSource = content.c_str();

    glShaderSource(shaderIndex, 1, &shaderSource, nullptr);
    GLenum result = glGetError();
    glCompileShader(shaderIndex);

    int shaderStatus;
    glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &shaderStatus);
    if (!shaderStatus)
    {
        char error[2048 + 1];
        glGetShaderInfoLog(shaderIndex, 2048, nullptr, error);

        throw std::runtime_error(error);
    }

    p_shaders.emplace_back(opengl::Shader());
    opengl::Shader* newShader = dynamic_cast<opengl::Shader*>(&p_shaders.back());
    newShader->p_type = Shader::ShaderType::Vertex;
    newShader->p_filename = filename;
    newShader->p_shaderIndex = shaderIndex;
    newShader->p_label = "VertexShader";

    *shader = static_cast<Shader*>(newShader);
}

}