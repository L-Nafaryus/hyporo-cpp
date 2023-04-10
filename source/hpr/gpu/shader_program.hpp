#pragma once

#include <hpr/containers.hpp>
#include <hpr/gpu/shader.hpp>
#include <hpr/exception.hpp>

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    struct ShaderProgramLinkError : public Exception
    {
        inline explicit ShaderProgramLinkError(std::source_location location = std::source_location::current()) : Exception {"Shader program link error", location} {}
        inline explicit ShaderProgramLinkError(const std::string& message, std::source_location location = std::source_location::current()) : Exception {message, location} {}
    };

    class ShaderProgram
    {

    protected:

        unsigned int p_index;

    public:

        // Constructors

        inline
        ShaderProgram() :
            p_index {0}
        {}

        virtual
        ~ShaderProgram() = default;

        [[nodiscard]]
        unsigned int index() const
        {
            return p_index;
        }

        void create()
        {
            p_index = glCreateProgram();
        }

        void attach(const Shader& shader)
        {
            glAttachShader(p_index, shader.index());
        }

        void detach(const Shader& shader)
        {
            glDetachShader(p_index, shader.index());
        }

        void link()
        {
            glLinkProgram(p_index);

            GLint status;
            glGetProgramiv(p_index, GL_LINK_STATUS, &status);

            if (status == GL_FALSE)
            {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetProgramInfoLog(p_index, 1024, &log_length, message);
                throw ShaderProgramLinkError(message);
            }
        }

        void destroy()
        {
            glDeleteProgram(p_index);
            p_index = 0;
        }

        void bind()
        {
            glUseProgram(p_index);
        }

        void unbind()
        {
            glUseProgram(0);
        }

        inline
        int uniformLocation(const std::string& label) const
        {
            return glGetUniformLocation(p_index, label.c_str());
        }

        template <typename T, std::convertible_to<T>... Args>
        inline
        void uniformValue(int location, T value, Args ...args)
        {
            auto arr = {value, static_cast<T>(args)...};
            if constexpr (std::is_same<T, int>::value)
            {
                switch (arr.size())
                {
                    case 1: glUniform1i(location, arr[0]); break;
                    case 2: glUniform2i(location, arr[0], arr[1]); break;
                    case 3: glUniform3i(location, arr[0], arr[1], arr[2]); break;
                    case 4: glUniform4i(location, arr[0], arr[1], arr[2], arr[3]); break;
                }
            }
            else if constexpr (std::is_same<T, unsigned int>::value)
            {
                switch (arr.size())
                {
                    case 1: glUniform1ui(location, arr[0]); break;
                    case 2: glUniform2ui(location, arr[0], arr[1]); break;
                    case 3: glUniform3ui(location, arr[0], arr[1], arr[2]); break;
                    case 4: glUniform4ui(location, arr[0], arr[1], arr[2], arr[3]); break;
                }
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                switch (arr.size())
                {
                    case 1: glUniform1f(location, arr[0]); break;
                    case 2: glUniform2f(location, arr[0], arr[1]); break;
                    case 3: glUniform3f(location, arr[0], arr[1], arr[2]); break;
                    case 4: glUniform4f(location, arr[0], arr[1], arr[2], arr[3]); break;
                }
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                switch (arr.size())
                {
                    case 1: glUniform1d(location, arr[0]); break;
                    case 2: glUniform2d(location, arr[0], arr[1]); break;
                    case 3: glUniform3d(location, arr[0], arr[1], arr[2]); break;
                    case 4: glUniform4d(location, arr[0], arr[1], arr[2], arr[3]); break;
                }
            }
            else
                throw std::runtime_error("Unsupported value type");
        }

        template <typename T, std::convertible_to<T>... Args>
        inline
        void uniformValue(const std::string& label, T value, Args ...args)
        {
            uniformValue(uniformLocation(label), value, std::forward<T>(args)...);
        }

        template <typename T, Size S>
        inline
        void uniformVector(int location, hpr::Size size, T* data)
        {
            if constexpr (std::is_same<T, int>::value)
            {
                switch (S)
                {
                    case 1: glUniform1iv(location, size, data); break;
                    case 2: glUniform2iv(location, size, data); break;
                    case 3: glUniform3iv(location, size, data); break;
                    case 4: glUniform4iv(location, size, data); break;
                }
            }
            else if constexpr (std::is_same<T, unsigned int>::value)
            {
                switch (S)
                {
                    case 1: glUniform1uiv(location, size, data); break;
                    case 2: glUniform2uiv(location, size, data); break;
                    case 3: glUniform3uiv(location, size, data); break;
                    case 4: glUniform4uiv(location, size, data); break;
                }
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                switch (S)
                {
                    case 1: glUniform1fv(location, size, data); break;
                    case 2: glUniform2fv(location, size, data); break;
                    case 3: glUniform3fv(location, size, data); break;
                    case 4: glUniform4fv(location, size, data); break;
                }
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                switch (size)
                {
                    case 1: glUniform1dv(location, size, data); break;
                    case 2: glUniform2dv(location, size, data); break;
                    case 3: glUniform3dv(location, size, data); break;
                    case 4: glUniform4dv(location, size, data); break;
                }
            }
            else
                throw std::runtime_error("Unsupported value type");
        }

        template <typename T, Size S>
        inline
        void uniformVector(const std::string& label, hpr::Size size, T* data)
        {
            uniformVector<T, S>(uniformLocation(label), size, data);
        }

        template <typename T, hpr::Size C, hpr::Size R>
        inline
        void uniformMatrix(int location, hpr::Size size, bool transpose, T* data)
        {
            if constexpr (std::is_same<T, float>::value)
            {
                if constexpr (C == 2 && R == 2)
                    glUniformMatrix2fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 3)
                    glUniformMatrix3fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 4)
                    glUniformMatrix4fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 2 && R == 3)
                    glUniformMatrix2x3fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 2)
                    glUniformMatrix3x2fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 2 && R == 4)
                    glUniformMatrix2x4fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 2)
                    glUniformMatrix4x2fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 4)
                    glUniformMatrix3x4fv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 3)
                    glUniformMatrix4x3fv(location, static_cast<GLsizei>(size), transpose, data);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                if constexpr (C == 2 && R == 2)
                    glUniformMatrix2dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 3)
                    glUniformMatrix3dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 4)
                    glUniformMatrix4dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 2 && R == 3)
                    glUniformMatrix2x3dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 2)
                    glUniformMatrix3x2dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 2 && R == 4)
                    glUniformMatrix2x4dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 2)
                    glUniformMatrix4x2dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 3 && R == 4)
                    glUniformMatrix3x4dv(location, static_cast<GLsizei>(size), transpose, data);
                else if constexpr (C == 4 && R == 3)
                    glUniformMatrix4x3dv(location, static_cast<GLsizei>(size), transpose, data);
            }
            else
                throw std::runtime_error("Unsupported value type");
        }

        template <hpr::Size C, hpr::Size R, typename T>
        inline
        void uniformMatrix(const std::string& label, hpr::Size size, bool transpose, T* data)
        {
            uniformMatrix<T, C, R>(uniformLocation(label), size, transpose, data);
        }
    };

}
