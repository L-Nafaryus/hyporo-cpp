#pragma once

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

class Shader
{

public:

    enum class Type
    {
        Vertex = GL_VERTEX_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEvaluation = GL_TESS_EVALUATION_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Compute = GL_COMPUTE_SHADER,
        Unknown = -1
    };

protected:

    std::string p_filename;
    std::string p_source;
    std::string p_label;
    Type p_type;

    unsigned int p_index;

public:

    // Constructors

    inline
    Shader() :
        p_filename {},
        p_source {},
        p_label {},
        p_type {Type::Unknown},
        p_index {0}
    {}

    inline
    Shader(Type type) :
        p_filename {},
        p_source {},
        p_label {},
        p_type {type}
    {}

    inline
    Shader(Type type, const std::string& source) :
        p_filename {},
        p_source {source},
        p_label {},
        p_type {type},
        p_index {0}
    {}

    virtual
    ~Shader() = default;

    // Member functions

    [[nodiscard]]
    std::string filename() const
    {
        return p_filename;
    }

    [[nodiscard]]
    std::string label() const
    {
        return p_label;
    }

    [[nodiscard]]
    Type type() const
    {
        return p_type;
    }

    [[nodiscard]]
    unsigned int index() const
    {
        return p_index;
    }

    void create(const std::string& label = "")
    {
        if (p_type == Type::Unknown)
            throw std::runtime_error("Unknown shader type");

        p_index = glCreateShader(static_cast<GLenum>(p_type));
        if (p_index == 0)
            throw std::runtime_error("Cannot create shader");

        const char* shaderSource = p_source.c_str();
        glShaderSource(p_index, 1, &shaderSource, nullptr);
        GLenum result = glGetError();
        glCompileShader(p_index);

        int shaderStatus;
        glGetShaderiv(p_index, GL_COMPILE_STATUS, &shaderStatus);
        if (!shaderStatus)
        {
            char error[2048 + 1];
            glGetShaderInfoLog(p_index, 2048, nullptr, error);

            throw std::runtime_error(error);
        }

        p_label = label;
    }

    void destroy()
    {
        glDeleteShader(p_index);
        p_index = 0;
    }
};

}