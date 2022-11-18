#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Shader : public Context
{
    friend class Device;

public:

    enum class ShaderType
    {
        Vertex,
        Geometry,
        Fragment,
        ShaderTypeCount
    };

protected:

    std::string p_filename;
    std::string p_label;
    ShaderType p_type;

public:

    // Constructors

    Shader();

    explicit
    Shader(DeviceAPI api);

    ~Shader() override;

    // Member functions

    [[nodiscard]]
    std::string filename() const;

    [[nodiscard]]
    std::string label() const;

    [[nodiscard]]
    ShaderType type() const;
};

} // end namespace hpr::gpu