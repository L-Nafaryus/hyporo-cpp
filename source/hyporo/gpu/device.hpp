#pragma once

#include "context.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture.hpp"

#include <vector>


namespace hpr::gpu
{

class Device : public Context
{

public:

    enum class CullMode
    {
        Front,
        Back,
        None
    };

protected:

    std::vector<Buffer> p_buffers;
    std::vector<Shader> p_shaders;
    std::vector<ShaderProgram> p_shaderPrograms;
    std::vector<Texture> p_textures;

protected:

    // Constructors

    Device();

    Device(DeviceAPI api);

    virtual ~Device();

public:

    // Global functions

    static void create(Device** device, DeviceAPI api);

    // Member functions

    // Setup

    virtual bool initialize() = 0;
    virtual bool destroy() = 0;

    // State

    virtual void faceCulling(bool enableFaceCulling, CullMode faceCullingMode = CullMode::None) = 0;

    // Buffers

    virtual void createVertexBuffer(Buffer **buffer, int size, char* data) = 0;
    virtual void createIndexBuffer(Buffer **buffer, int size, char* data) = 0;
    virtual void useVertexBuffer(Buffer* buffer, int stride, int offset);
    virtual void useIndexBuffer(Buffer* buffer, int offset);
    virtual bool destroyBuffer(Buffer*& buffer);

    // Shaders

    virtual void createVertexShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual void createFragmentShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual void createGeometryShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual bool destroyShader(Shader*& shader);

    // Shader programs

    virtual void createShaderProgram(ShaderProgram** program) = 0;
    virtual void attachShader(ShaderProgram* program, Shader* shader);
    virtual void linkProgram(ShaderProgram* program);
    virtual void useShaderProgram(ShaderProgram* program);
    virtual void destroyShaderProgram(ShaderProgram*& program, bool withShaders = false);

    // Textures

    virtual void createTexture(Texture** texture, const std::string& filename) = 0;
    virtual void useTexture(Texture* texture, int slot);
    virtual void destroyTexture(Texture*& texture);
};

}
