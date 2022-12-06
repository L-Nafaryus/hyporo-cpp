#pragma once

#include "../device.hpp"

#include <functional>


namespace hpr::gpu::opengl
{

class Device : public gpu::Device
{

protected:

    bool p_isInitialized;

public:

    // Constructors

    Device();

    ~Device() override;


    // Member functions

    // Setup

    bool initialize() override;
    bool destroy() override;

    static
    bool loadLoader();

    // State

    void faceCulling(bool enableFaceCulling, CullMode faceCullingMode) override;

    // Render targets

    void createScreenRenderTarget(RenderTarget** target, Window* window) override;
    void createFramebufferRenderTarget(RenderTarget** target, int width, int height) override;
    void createSubRenderTarget(RenderTarget** target, RenderTarget* parent, int x, int y, int width, int height) override;
    void moveRenderTarget(RenderTarget* target, int x, int y) override;
    void scaleRenderTarget(RenderTarget* target, int width, int height) override;
    void destroyRenderTarget(RenderTarget*& target) override;

    // Buffers

    virtual
    void createVertexBuffer(Buffer **buffer, int size, char* data);
    virtual
    void createIndexBuffer(Buffer **buffer, int size, char* data);
    virtual
    void createUniformBuffer(Buffer **buffer, int size, char* data);
    virtual
    void useVertexBuffer(Buffer* buffer, int stride, int offset);
    virtual
    void useIndexBuffer(Buffer* buffer, int offset);
    virtual
    void useUniformBuffer(Buffer* buffer, int slot);
    virtual
    void editBuffer(Buffer* buffer, char* data, int size, int offset);
    virtual
    void editBuffer(Buffer* buffer, char* data);
    virtual
    void destroyBuffer(Buffer*& buffer);

    // Shaders

    virtual
    void createVertexShader(Shader** shader, const std::string& filename, const std::string& label);
    virtual
    void createFragmentShader(Shader** shader, const std::string& filename, const std::string& label);
    virtual
    void createGeometryShader(Shader** shader, const std::string& filename, const std::string& label);
    virtual
    void destroyShader(Shader* shader);

    // Shader programs

    virtual
    void createShaderProgram(ShaderProgram** program);
    virtual
    void attachShader(ShaderProgram* program, Shader* shader);
    virtual
    void linkProgram(ShaderProgram* program);
    virtual
    void useShaderProgram(ShaderProgram* program);
    virtual
    void destroyShaderProgram(ShaderProgram*& program, bool withShaders);

    // Textures

    void createTexture(Texture** texture, const std::string& filename) override;
    void useTexture(Texture* texture, int slot) override;
    void destroyTexture(Texture*& texture) override;

    //

    void Draw(int numFaces, int indexOffset, int vertexOffset);
};

}