#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Shader : public Context
{
protected:

    std::string p_filename;
    std::string p_label;

public:

    enum class ShaderType
    {
        Vertex,
        Geometry,
        Fragment,
        ShaderTypeCount
    };

    // Constructors

    Shader();

    Shader(DeviceAPI api);

    virtual ~Shader();

    // Member functions

    const std::string filename() const;

    const std::string label() const;

    const ShaderType shaderType() const;
};

} // end namespace hpr::gpu