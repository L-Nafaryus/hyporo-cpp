#pragma once

#include "buffer_object.hpp"

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    class ArrayObject
    {


    protected:

        unsigned int p_index;
        int p_size;
        int p_stride;
        bool p_binded;

    public:

        inline
        ArrayObject() :
                p_index {0},
                p_size {0},
                p_stride {0},
                p_binded {false}
        {}

        virtual
        ~ArrayObject() = default;

        [[nodiscard]]
        int size() const
        {
            return p_size;
        }

        [[nodiscard]]
        unsigned int index() const
        {
            return p_index;
        }

        void create()
        {
            glGenVertexArrays(1, &p_index);
        }

        void bind()
        {
            glBindVertexArray(p_index);
            p_binded = true;
        }

        void unbind()
        {
            glBindVertexArray(0);
            p_binded = false;
        }

        bool binded() const
        {
            return p_binded;
        }

        void destroy()
        {
            glDeleteVertexArrays(1, &p_index);
        }

        void attribPointer(BufferObject& buffer, unsigned int location, unsigned int size)
        {
            if (buffer.type() == BufferObject::Type::Unknown)
                throw std::runtime_error("Unknown buffer type");
            if (!binded())
                throw std::runtime_error("ArrayObject is not binded");
            if (!buffer.valid())
                throw std::runtime_error("BufferObject is invalid");

            buffer.bind();
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, sizeof(float) * buffer.offset(), static_cast<void*>(nullptr));
            glEnableVertexAttribArray(location);
            buffer.unbind();
        }

        void draw()
        {

        }

        inline
        bool valid() const
        {
            return p_index > 0;
        }
    };

}
