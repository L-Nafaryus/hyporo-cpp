#pragma once

#include <hpr/containers.hpp>


#include <string>
#ifndef __gl_h_
#include <glad/glad.h>
#endif


namespace hpr::gpu
{

class BufferObject
{

public:

    enum Type
    {
        Vertex = 0x8892, // GL_ARRAY_BUFFER,
        Index = 0x8893, // GL_ELEMENT_ARRAY_BUFFER,
        Uniform = 0x8A11, // GL_UNIFORM_BUFFER,
        Unknown = -1
    };

protected:

    Type p_type;
    unsigned int p_index;
    int p_size;
    unsigned int p_offset;
    bool p_binded;

public:

    inline
    BufferObject() :
        p_type {Type::Unknown},
        p_index {0},
        p_size {0},
        p_offset {0},
        p_binded {false}
    {}

    explicit inline
    BufferObject(Type type) :
        p_type {type},
        p_index {0},
        p_size {0},
        p_offset {0},
        p_binded {false}
    {}

    virtual
    ~BufferObject() = default;

    [[nodiscard]]
    int size() const
    {
        return p_size;
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

    [[nodiscard]]
    unsigned int offset() const
    {
        return p_offset;
    }

    void bind()
    {
        glBindBuffer((GLenum)p_type, p_index);
        p_binded = true;
    }

    void unbind()
    {
        glBindBuffer((GLenum)p_type, 0);
        p_binded = false;
    }

    [[nodiscard]]
    bool binded() const
    {
        return p_binded;
    }

    template <typename T>
    void create(const darray<T>& data, unsigned int offset = 0)
    {
        if (p_type == Type::Unknown)
            throw std::runtime_error("Unknown buffer type");

        unsigned int drawType;

        if (p_type == Type::Uniform)
            drawType = GL_DYNAMIC_DRAW;
        else
            drawType = GL_STATIC_DRAW;

        glGenBuffers(1, &p_index);
        bind();
        glBufferData(static_cast<GLenum>(p_type), sizeof(T) * data.size(), data.data(), drawType);
        unbind();

        p_offset = offset;
        p_size = data.size();
    }

    template <typename T>
    void edit(const darray<T>& data, unsigned int offset = 0)
    {
        if (p_type == Type::Unknown)
            throw std::runtime_error("Unknown buffer type");

        bind();
        glBufferSubData(p_type, offset, sizeof(T) * data.size(), data.data());
        unbind();

        p_offset = offset;
        p_size = data.size();
    }

    void destroy()
    {
        if (p_type == Type::Unknown)
            throw std::runtime_error("Unknown buffer type");

        glDeleteBuffers(1, &p_index);
    }

    [[nodiscard]]
    inline
    bool valid() const
    {
        return p_index > 0;
    }
};

}