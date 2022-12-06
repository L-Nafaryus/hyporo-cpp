#pragma once

#include "context.hpp"
#include "shader.hpp"

#include "../containers/array.hpp"


namespace hpr::gpu
{

class ShaderProgram : public Context
{
    friend class Device;

protected:

    sarray<Shader*, (size_t)Shader::ShaderType::ShaderTypeCount> p_slots;
    bool p_isLinked;

public:

    // Constructors

    ShaderProgram();

    explicit
    ShaderProgram(DeviceAPI api);

    ~ShaderProgram() override;

    // Member functions

    const Shader* getShader(Shader::ShaderType type) const;
};

}