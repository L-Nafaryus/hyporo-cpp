#pragma once

#include "context.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "render_target.hpp"

#include "../containers/array.hpp"

#include "../window_system/window_system.hpp"


namespace hpr::gpu
{

class Device : public Context
{

public:

    enum class CullMode
    {
        Front,
        Back,
        FrontAndBack,
        None
    };

protected:

    darray<Buffer*> p_buffers;
    darray<Shader*> p_shaders;
    darray<ShaderProgram*> p_shaderPrograms;
    darray<Texture*> p_textures;

    Buffer* p_currentVertexBuffer;
    Buffer* p_currentIndexBuffer;
    Buffer* p_currentUniformBuffer;
    ShaderProgram* p_currentShaderProgram;

    darray<RenderTarget*> p_renderTargets;
protected:

    // Constructors

    Device();

    explicit
    Device(DeviceAPI api);

    ~Device() override;

public:

    // Global functions

    static
    void create(Device** device, DeviceAPI api);

    // Member functions

    // Setup

    virtual
    bool initialize() = 0;
    virtual
    bool destroy() = 0;

    // State

    virtual
    void faceCulling(bool enableFaceCulling, CullMode faceCullingMode) = 0;

    // Render targets

    virtual
    void createScreenRenderTarget(RenderTarget** target, Window* window) = 0;
    virtual
    void createFramebufferRenderTarget(RenderTarget** target, int width, int height) = 0;
    virtual
    void createSubRenderTarget(RenderTarget** target, RenderTarget* parent, int x, int y, int width, int height) = 0;
    virtual
    void moveRenderTarget(RenderTarget* target, int x, int y);
    virtual
    void scaleRenderTarget(RenderTarget* target, int width, int height);
    virtual
    void destroyRenderTarget(RenderTarget*& target);

    // Buffers

    virtual
    void createVertexBuffer(Buffer **buffer, int size, char* data) = 0;
    virtual
    void createIndexBuffer(Buffer **buffer, int size, char* data) = 0;
    virtual
    void createUniformBuffer(Buffer **buffer, int size, char* data) = 0;
    virtual
    void useVertexBuffer(Buffer* buffer, int stride, int offset);
    virtual
    void useIndexBuffer(Buffer* buffer, int offset);
    virtual
    void useUniformBuffer(Buffer* buffer, int slot);
    virtual
    void editBuffer(Buffer* buffer, char* data, int size, int offset) = 0;
    virtual
    void editBuffer(Buffer* buffer, char* data) = 0;
    virtual
    void destroyBuffer(Buffer*& buffer);
    Buffer* activeBuffer(Buffer::BufferType type);


    // Shaders

    virtual
    void createVertexShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual
    void createFragmentShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual
    void createGeometryShader(Shader** shader, const std::string& filename, const std::string& label) = 0;
    virtual
    void destroyShader(Shader* shader);

    // Shader programs

    virtual
    void createShaderProgram(ShaderProgram** program) = 0;
    virtual
    void attachShader(ShaderProgram* program, Shader* shader);
    virtual
    void linkProgram(ShaderProgram* program);
    virtual
    void useShaderProgram(ShaderProgram* program);
    virtual
    void destroyShaderProgram(ShaderProgram*& program, bool withShaders);

    // Textures

    virtual
    void createTexture(Texture** texture, const std::string& filename) = 0;
    virtual
    void useTexture(Texture* texture, int slot) = 0;
    virtual
    void destroyTexture(Texture*& texture);
};

}
