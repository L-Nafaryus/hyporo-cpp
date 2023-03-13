#pragma once

#include <hpr/gpu/buffer_object.hpp>

#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    class ArrayObject
    {
    public:

        enum Mode
        {
            Points = GL_POINTS,
            LineStrip = GL_LINE_STRIP,
            LineLoop = GL_LINE_LOOP,
            Lines = GL_LINES,
            LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
            LinesAdjacency = GL_LINES_ADJACENCY,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
            Triangles = 0x0004, //GL_TRIANGLES,
            TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
            TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
            Patches = GL_PATCHES
        };

    protected:

        GLuint p_index;
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

        void attribPointer(BufferObject& buffer, unsigned int location, int size)
        {
            if (buffer.type() == BufferObject::Type::Unknown)
                throw std::runtime_error("Unknown buffer type");
            if (!binded())
                throw std::runtime_error("ArrayObject is not binded");
            if (!buffer.valid())
                throw std::runtime_error("BufferObject is invalid");

            buffer.bind();
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, sizeof(float) * buffer.offset(), static_cast<void*>(nullptr));
            buffer.unbind();
        }

        void drawElements(Mode mode, int count) const
        {
            glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
        }

        void drawArrays(Mode mode, int count) const
        {
            glDrawArrays(mode, 0, count);
        }

        inline
        bool valid() const
        {
            return p_index > 0;
        }
    };

}
