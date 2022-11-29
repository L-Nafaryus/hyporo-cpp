
#include "device.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "render_target.hpp"

#include "io/io.hpp"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

#include <GLFW/glfw3.h>


namespace hpr::gpu::opengl
{

Device::Device() :
    gpu::Device {DeviceAPI::OpenGL},
    p_isInitialized {false}
{}

Device::~Device() = default;

// Setup

bool Device::initialize()
{
    return p_isInitialized = true;
}

bool Device::destroy()
{
    return p_isInitialized = false;
}

bool Device::loadLoader()
{
    return !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
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

// Render targets

void Device::createScreenRenderTarget(gpu::RenderTarget** target, Window* window)
{
    if (target == nullptr)
        throw std::invalid_argument("Invalid parameter");
    *target = nullptr;
    p_renderTargets.push(new opengl::RenderTarget());
    auto* newTarget = dynamic_cast<opengl::RenderTarget*>(p_renderTargets.back());

    newTarget->p_type = RenderTarget::Type::Screen;
    newTarget->p_posX = 0;
    newTarget->p_posY = 0;
    newTarget->p_width = window->width();
    newTarget->p_height = window->height();

    *target = newTarget;
}

void Device::createFramebufferRenderTarget(gpu::RenderTarget** target, int width, int height)
{
    if (target == nullptr)
        throw std::invalid_argument("Invalid parameter");
    unsigned int texture;
    glGenTextures(GL_TEXTURE_2D, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);

    p_renderTargets.push(new opengl::RenderTarget());
    auto* newTarget = dynamic_cast<opengl::RenderTarget*>(p_renderTargets.back());

    newTarget->p_type = RenderTarget::Type::Framebuffer;
    newTarget->p_posX = 0;
    newTarget->p_posY = 0;
    newTarget->p_width = width;
    newTarget->p_height = height;
    newTarget->p_frameBufferIndex = framebuffer;
    newTarget->p_textureIndex = texture;

    *target = newTarget;
}

void Device::createSubRenderTarget(gpu::RenderTarget** target, RenderTarget* parent, int x, int y, int width, int height)
{

}

void Device::moveRenderTarget(gpu::RenderTarget* target, int x, int y)
{

}

void Device::scaleRenderTarget(gpu::RenderTarget* target, int width, int height)
{

}

void Device::destroyRenderTarget(gpu::RenderTarget*& target)
{

}

// Buffers


void Device::createVertexBuffer(gpu::Buffer **buffer, int size, char *data)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *buffer = nullptr;
    p_buffers.push(new opengl::Buffer());
    auto* newBuffer = dynamic_cast<opengl::Buffer*>(p_buffers.back());

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
    p_buffers.push(new opengl::Buffer());
    auto* newBuffer = dynamic_cast<opengl::Buffer*>(p_buffers.back());

    newBuffer->p_type = Buffer::BufferType::Index;
    newBuffer->p_size = size;

    glGenVertexArrays(1, &newBuffer->p_vertexArrayIndex);
    glBindVertexArray(newBuffer->p_vertexArrayIndex);

    glGenBuffers(1, &newBuffer->p_bufferIndex);
    glBindBuffer(GL_UNIFORM_BUFFER, newBuffer->p_bufferIndex);
    glBufferData(GL_UNIFORM_BUFFER, size, (void*)data, GL_STATIC_DRAW);

    *buffer = static_cast<gpu::Buffer*>(newBuffer);
}

void Device::createUniformBuffer(gpu::Buffer** buffer, int size, char* data)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *buffer = nullptr;
    p_buffers.push(new opengl::Buffer());
    auto* newBuffer = dynamic_cast<opengl::Buffer*>(p_buffers.back());

    newBuffer->p_type = Buffer::BufferType::Uniform;
    newBuffer->p_size = size;

    glGenBuffers(1, &newBuffer->p_bufferIndex);
    glBindBuffer(GL_UNIFORM_BUFFER, newBuffer->p_bufferIndex);
    glBufferData(GL_UNIFORM_BUFFER, size, (void*)data, GL_DYNAMIC_DRAW);

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
        auto* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);

        if (curBuffer->p_type == Buffer::BufferType::Vertex && p_currentVertexBuffer != buffer)
        {
            glBindVertexArray(curBuffer->p_vertexArrayIndex);
            glBindBuffer(GL_ARRAY_BUFFER, curBuffer->p_bufferIndex);

            auto* curIndexBuffer = dynamic_cast<opengl::Buffer*>(p_currentIndexBuffer);

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
        auto* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);

        if (curBuffer->p_type == Buffer::BufferType::Index && p_currentVertexBuffer != buffer)
        {
            glBindVertexArray(curBuffer->p_vertexArrayIndex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curBuffer->p_bufferIndex);
        }
    }
    gpu::Device::useIndexBuffer(buffer, offset);
}

void Device::useUniformBuffer(gpu::Buffer* buffer, int slot)
{
    if (buffer == nullptr)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    else
    {
        auto* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);
        glBindBufferRange(GL_UNIFORM_BUFFER, slot, curBuffer->p_bufferIndex, 0, buffer->size());
    }
    gpu::Device::useUniformBuffer(buffer, slot);
}

void Device::editBuffer(gpu::Buffer* buffer, char* data, int size, int offset)
{
    if (!checkCompability(buffer))
        throw std::runtime_error("Incompatible platform");
    if (buffer == nullptr || data == nullptr)
        throw std::invalid_argument("Invalid argument");
    if (size + offset > buffer->size() || size < 0 || offset < 0)
        throw std::out_of_range("Out of bounds");

    auto* prevBuffer = dynamic_cast<opengl::Buffer*>(activeBuffer(buffer->type()));

    auto* buffer_ = dynamic_cast<opengl::Buffer*>(buffer);
    glBindBuffer(buffer_->target(), buffer_->p_bufferIndex);
    glBufferSubData(buffer_->target(), offset, size, data);

    if (prevBuffer != buffer_)
        glBindBuffer(buffer_->target(), prevBuffer ? prevBuffer->p_bufferIndex : 0);

}

void Device::editBuffer(gpu::Buffer* buffer, char* data)
{
    editBuffer(buffer, data, buffer->size(), 0);
}

void Device::destroyBuffer(gpu::Buffer *&buffer)
{
    if (buffer == nullptr)
        throw std::invalid_argument("Invalid parameter");

    auto* curBuffer = dynamic_cast<opengl::Buffer*>(buffer);
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

    p_shaders.push(new opengl::Shader());
    auto* newShader = dynamic_cast<opengl::Shader*>(p_shaders.back());
    newShader->p_type = Shader::ShaderType::Vertex;
    newShader->p_filename = filename;
    newShader->p_shaderIndex = shaderIndex;
    newShader->p_label = "VertexShader";

    *shader = static_cast<Shader*>(newShader);
}


void Device::createFragmentShader(gpu::Shader **shader, const std::string &filename, const std::string &label)
{
    if (shader == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *shader = nullptr;

    unsigned int shaderIndex = glCreateShader(GL_FRAGMENT_SHADER);
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

    p_shaders.push(new opengl::Shader());
    auto* newShader = dynamic_cast<opengl::Shader*>(p_shaders.back());
    newShader->p_type = Shader::ShaderType::Fragment;
    newShader->p_filename = filename;
    newShader->p_shaderIndex = shaderIndex;
    newShader->p_label = "FragmentShader";

    *shader = static_cast<Shader*>(newShader);
}


void Device::createGeometryShader(gpu::Shader **shader, const std::string &filename, const std::string &label)
{
    if (shader == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *shader = nullptr;

    unsigned int shaderIndex = glCreateShader(GL_GEOMETRY_SHADER);
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

    p_shaders.push(new opengl::Shader());
    auto* newShader = dynamic_cast<opengl::Shader*>(p_shaders.back());
    newShader->p_type = Shader::ShaderType::Geometry;
    newShader->p_filename = filename;
    newShader->p_shaderIndex = shaderIndex;
    newShader->p_label = "FragmentShader";

    *shader = static_cast<gpu::Shader*>(newShader);
}

void Device::destroyShader(gpu::Shader* shader)
{
    if (shader == nullptr)
        throw std::invalid_argument("Invalid parameter");

    auto* shader_ = dynamic_cast<opengl::Shader*>(shader);
    glDeleteShader(shader_->p_shaderIndex);

    gpu::Device::destroyShader(shader);
}

void Device::createShaderProgram(gpu::ShaderProgram** program)
{
    if (program == nullptr)
        throw std::invalid_argument("Invalid parameter");

    *program = nullptr;

    p_shaderPrograms.push(new opengl::ShaderProgram());
    auto* newProgram = dynamic_cast<opengl::ShaderProgram*>(p_shaderPrograms.back());
    newProgram->p_shaderProgramIndex = glCreateProgram();

    *program = static_cast<gpu::ShaderProgram*>(newProgram);
}

void Device::attachShader(gpu::ShaderProgram* program, gpu::Shader* shader)
{
    gpu::Device::attachShader(program, shader);
    auto* program_ = dynamic_cast<opengl::ShaderProgram*>(program);
    auto* shader_ = dynamic_cast<opengl::Shader*>(shader);
    glAttachShader(program_->p_shaderProgramIndex, shader_->p_shaderIndex);
}


void Device::linkProgram(gpu::ShaderProgram* program)
{
    gpu::Device::linkProgram(program);
    auto* program_ = dynamic_cast<opengl::ShaderProgram*>(program);
    glLinkProgram(program_->p_shaderProgramIndex);

    GLint status;
    glGetProgramiv(program_->p_shaderProgramIndex, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
        throw std::runtime_error("Shader program link error");
}

void Device::useShaderProgram(gpu::ShaderProgram* program)
{
    gpu::ShaderProgram* currentProgram = p_currentShaderProgram;
    gpu::Device::useShaderProgram(program);

    if (currentProgram == program)
        return;
    if (program == nullptr)
        glUseProgram(0);
    else
    {
        auto* program_ = dynamic_cast<opengl::ShaderProgram*>(program);
        glUseProgram(program_->p_shaderProgramIndex);
    }
}

void Device::destroyShaderProgram(gpu::ShaderProgram*& program, bool withShaders)
{
    if (program == nullptr)
        throw std::invalid_argument("Invalid parameter");

    auto* program_ = dynamic_cast<opengl::ShaderProgram*>(program);

    for (int n = 0; n < static_cast<int>(Shader::ShaderType::ShaderTypeCount); ++n)
    {
        auto* shader = dynamic_cast<opengl::Shader*>(program_->p_slots[n]);
        if (shader != nullptr)
            glDetachShader(program_->p_shaderProgramIndex, shader->p_shaderIndex);
    }
    glDeleteProgram(program_->p_shaderProgramIndex);
    gpu::Device::destroyShaderProgram(program, withShaders);
}

void Device::createTexture(gpu::Texture** texture, const std::string& filename)
{
    if (texture == nullptr)
        throw std::invalid_argument("Invalid parameter");
    if (filename.empty())
        throw std::invalid_argument("Invalid parameter");

    *texture = nullptr;
    bool alphaChannel = true;
    int internalFormat = GL_RGBA;
    int imageFormat = GL_RGBA;
    int width;
    int height;
    int channelsCount;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* source = stbi_load(filename.c_str(), &width, &height, &channelsCount, 0);

    if (!source)
        throw std::runtime_error("Failed to load texture source");
    else
    {
        p_textures.push(new opengl::Texture());
        auto* texture_ = dynamic_cast<opengl::Texture*>(p_textures.back());
        texture_->p_filename = filename;
        texture_->p_width = width;
        texture_->p_height = height;

        glGenTextures(1, &texture_->p_textureIndex);
        glBindTexture(GL_TEXTURE_2D, texture_->p_textureIndex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (alphaChannel)
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        else
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, source);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        *texture = static_cast<gpu::Texture*>(texture_);
    }

    stbi_image_free(source);
}

void Device::useTexture(gpu::Texture* texture, int slot)
{
    //gpu::Device::useTexture(texture, slot);

    if (texture == nullptr)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        auto* texture_ = dynamic_cast<opengl::Texture*>(texture);
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture_->p_textureIndex);
    }
}

void Device::destroyTexture(gpu::Texture*& texture)
{
    if (texture == nullptr)
        throw std::invalid_argument("Invalid parameter");

    auto* texture_ = dynamic_cast<opengl::Texture*>(texture);

    glDeleteTextures(1, &texture_->p_textureIndex);
    gpu::Device::destroyTexture(texture);
}


void Device::Draw(int numFaces, int indexOffset, int vertexOffset)
{
    if (p_currentVertexBuffer != nullptr)
        glDrawElementsBaseVertex(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_SHORT, (void*)(indexOffset * 2), vertexOffset);
}

}