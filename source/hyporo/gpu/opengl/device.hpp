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

    void createVertexBuffer(Buffer **buffer, int size, char* data) override;
    void createIndexBuffer(Buffer **buffer, int size, char* data) override;
    void createUniformBuffer(Buffer **buffer, int size, char* data) override;
    void useVertexBuffer(Buffer* buffer, int stride, int offset) override;
    void useIndexBuffer(Buffer* buffer, int offset) override;
    void useUniformBuffer(Buffer* buffer, int slot) override;
    void editBuffer(Buffer* buffer, char* data, int size, int offset) override;
    void editBuffer(Buffer* buffer, char* data) override;
    void destroyBuffer(Buffer*& buffer) override;

    // Shaders

    void createVertexShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void createFragmentShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void createGeometryShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void destroyShader(Shader* shader) override;

    // Shader programs

    void createShaderProgram(ShaderProgram** program) override;
    void attachShader(ShaderProgram* program, Shader* shader) override;
    void linkProgram(ShaderProgram* program) override;
    void useShaderProgram(ShaderProgram* program) override;
    void destroyShaderProgram(ShaderProgram*& program, bool withShaders) override;

    // Textures

    void createTexture(Texture** texture, const std::string& filename) override;
    void useTexture(Texture* texture, int slot) override;
    void destroyTexture(Texture*& texture) override;

    //

    void Draw(int numFaces, int indexOffset, int vertexOffset);
};

}