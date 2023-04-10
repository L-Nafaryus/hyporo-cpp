#pragma once

#include <hpr/gpu.hpp>
#include <hpr/math.hpp>

using namespace hpr;

class Drawable
{

protected:

    gpu::ArrayObject p_arrayObject;

    gpu::BufferObject p_vertexBuffer;
    gpu::BufferObject p_normalBuffer;
    gpu::BufferObject p_colorBuffer;
    gpu::BufferObject p_indexBuffer;

    gpu::ArrayObject::Mode p_renderMode;

    hpr::vec4 p_color;

public:

    gpu::ArrayObject& array()
    {
        return p_arrayObject;
    }
    friend constexpr void swap(Drawable& main, Drawable& other)
    {
        using std::swap;
        //swap(main.p)
    }

    Drawable() :
        p_arrayObject {},
        p_vertexBuffer {gpu::BufferObject::Vertex},
        p_normalBuffer {gpu::BufferObject::Vertex},
        p_colorBuffer {gpu::BufferObject::Vertex},
        p_indexBuffer {gpu::BufferObject::Index},
        p_renderMode {gpu::ArrayObject::Triangles},
        p_color {0.7, 0.7, 0.7}
    {}

    inline
    Drawable(const Drawable& drawable) = default;

    virtual
    ~Drawable() = default;

    void destroy()
    {
        p_arrayObject.destroy();
        p_vertexBuffer.destroy();
        p_normalBuffer.destroy();
        p_colorBuffer.destroy();
        p_indexBuffer.destroy();
    }

    constexpr virtual
    void renderMode(gpu::ArrayObject::Mode mode)
    {
        p_renderMode = mode;
    }

    [[nodiscard]]
    constexpr virtual
    gpu::ArrayObject::Mode renderMode() const
    {
        return p_renderMode;
    }

    template <IsVector V>
    inline
    void addVertices(const darray<V>& vertices)
    {
        if (!p_arrayObject.valid())
            p_arrayObject.create();
        p_arrayObject.bind();

        darray<float> data;
        data.resize(vertices.size() * vertices[0].size());
        for (auto v : vertices) for (auto c : v) data.push(c);
        p_vertexBuffer.create(data);

        p_arrayObject.attribPointer(p_vertexBuffer, 0, vertices[0].size());
        p_arrayObject.unbind();
    }

    template <IsVector V>
    inline
    void editVertices(const darray<V>& vertices)
    {
        if (!p_vertexBuffer.valid())
            throw std::runtime_error("Invalid buffer object");
        p_arrayObject.bind();

        darray<float> data;
        data.resize(vertices.size() * vertices[0].size());
        for (auto v : vertices) for (auto c : v) data.push(c);

        p_vertexBuffer.edit(data);

        p_arrayObject.unbind();
    }

    template <IsVector V>
    inline
    void addNormals(const darray<V>& normals)
    {
        if (!p_arrayObject.valid())
            p_arrayObject.create();
        p_arrayObject.bind();

        darray<float> data;
        data.resize(normals.size() * normals[0].size());
        for (auto v : normals) for (auto c : v) data.push(c);
        p_normalBuffer.create(data);

        p_arrayObject.attribPointer(p_normalBuffer, 1, normals[0].size());
        p_arrayObject.unbind();
    }

    template <IsVector V>
    inline
    void addColors(const darray<V>& colors)
    {
        if (!p_arrayObject.valid())
            p_arrayObject.create();
        p_arrayObject.bind();

        darray<float> data;
        data.resize(colors.size() * colors[0].size());
        for (auto v : colors) for (auto c : v) data.push(c);
        p_colorBuffer.create(data);

        p_arrayObject.attribPointer(p_colorBuffer, 2, colors[0].size());
        p_arrayObject.unbind();
    }

    template <IsVector V>
    inline
    void editColors(const darray<V>& colors)
    {
        if (!p_colorBuffer.valid())
            throw std::runtime_error("Invalid buffer object");
        p_arrayObject.bind();

        darray<float> data;
        data.resize(colors.size() * colors[0].size());
        for (auto v : colors) for (auto c : v) data.push(c);

        p_colorBuffer.edit(data);

        p_arrayObject.unbind();
    }

    template <IsVector V>
    inline
    void addIndices(const darray<V>& indices)
    {
        if (!p_arrayObject.valid())
            p_arrayObject.create();
        p_arrayObject.bind();

        darray<unsigned int> data;
        data.resize(indices.size() * indices[0].size());
        for (auto v : indices) for (auto c : v) data.push(c);
        p_indexBuffer.create(data);

        p_arrayObject.unbind();
    }

    inline virtual
    void render(gpu::ShaderProgram* shaderProgram, gpu::ArrayObject::Mode mode)
    {
        shaderProgram->bind();
        //std::cout << p_shaderProgram->index() << std::endl;
        p_arrayObject.bind();

        if (!p_colorBuffer.valid()) {

            shaderProgram->uniformVector<float, 4>("objectColor", 1, p_color.data());
        }
        if (p_indexBuffer.valid())
        {
            p_indexBuffer.bind();
            p_arrayObject.drawElements(mode, p_indexBuffer.size());
        }
        else
        {
            p_arrayObject.drawArrays(mode, p_vertexBuffer.size());
        }
        p_arrayObject.unbind();
        shaderProgram->unbind();
    }

    inline virtual
    void render(gpu::ShaderProgram* shaderProgram)
    {
        render(shaderProgram, p_renderMode);
    }

    inline virtual
    void color(const hpr::vec4& color)
    {
        p_color = color;
    }

    inline virtual
    hpr::vec4& color()
    {
        return p_color;
    }
};
