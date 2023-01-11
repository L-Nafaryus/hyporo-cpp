#pragma once

#include "../containers.hpp"

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    class ShaderProgram
    {

    protected:

        unsigned int p_index;
        darray<Shader> p_shaders;

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

        darray<Shader> shaders()
        {
            return p_shaders;
        }

        void create(const std::string& label = "")
        {
            p_index = glCreateProgram();
        }

        void attach(const Shader& shader)
        {
            glAttachShader(p_index, shader.index());
            p_shaders.push(shader);
        }

        void detach(const Shader& shader)
        {
            // WARNING: segfault, destroy_at (char)
            p_shaders.remove([shader](const Shader& _shader)
            {
                return shader.index() == _shader.index();
            });

            glDetachShader(p_index, shader.index());
        }

        void link()
        {
            glLinkProgram(p_index);

            GLint status;
            glGetProgramiv(p_index, GL_LINK_STATUS, &status);

            if (status == GL_FALSE)
                throw std::runtime_error("Shader program link error");
        }

        void destroy()
        {
            //for (auto& shader : p_shaders)
            //    detach(shader);
            glDeleteShader(p_index);
        }

        void bind()
        {
            glUseProgram(p_index);
        }

        void unbind()
        {
            glUseProgram(0);
        }
    };

}
