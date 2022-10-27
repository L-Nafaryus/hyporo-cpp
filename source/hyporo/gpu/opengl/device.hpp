#pragma once

#include "../device.hpp"


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

    // State

    void faceCulling(bool enableFaceCulling, CullMode faceCullingMode = CullMode::None) override;

    // Buffers

    void createVertexBuffer(Buffer **buffer, int size, char* data) override;
    void createIndexBuffer(Buffer **buffer, int size, char* data) override;
    void useVertexBuffer(Buffer* buffer, int stride, int offset) override;
    void useIndexBuffer(Buffer* buffer, int offset) override;
    void destroyBuffer(Buffer*& buffer) override;

    // Shaders

    void createVertexShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void createFragmentShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void createGeometryShader(Shader** shader, const std::string& filename, const std::string& label) override;
    void destroyShader(Shader*& shader) override;

    // Shader programs

    virtual void createShaderProgram(ShaderProgram** program);
    virtual void attachShader(ShaderProgram* program, Shader* shader);
    virtual void linkProgram(ShaderProgram* program);
    virtual void useShaderProgram(ShaderProgram* program);
    virtual void destroyShaderProgram(ShaderProgram*& program, bool withShaders = false);

    // Textures

    virtual void createTexture(Texture** texture, const std::string& filename);
    virtual void useTexture(Texture* texture, int slot);
    virtual void destroyTexture(Texture*& texture);
};

}