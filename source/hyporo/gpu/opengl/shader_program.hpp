#pragma once

#include "../shader_program.hpp"


namespace hpr::gpu::opengl
{

// Forward declarations

class Shader;

//

class ShaderProgram : public gpu::ShaderProgram
{
    friend class Device;

protected:

    unsigned int p_shaderProgramIndex;

public:

    ShaderProgram();

    ~ShaderProgram();

protected:

    Shader* shader(gpu::Shader::ShaderType type);

};

}
