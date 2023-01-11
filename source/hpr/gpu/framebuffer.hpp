#pragma once

#include "texture.hpp"
#include "renderbuffer.hpp"

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

class Framebuffer
{

protected:

    Texture p_texture;
    Renderbuffer p_renderbuffer;
    unsigned int p_index;
    bool p_binded;

public:

    inline
    Framebuffer() :
        p_index {0},
        p_texture {},
        p_renderbuffer {}
    {}

    virtual
    ~Framebuffer() = default;

    [[nodiscard]]
    unsigned int index() const
    {
        return p_index;
    }

    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, p_index);
        p_binded = true;
    }

    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        p_binded = false;
    }

    bool binded() const
    {
        return p_binded;
    }

    void attach(const Texture& texture)
    {
        if (!binded() && valid())
            std::runtime_error("Framebuffer not binded or invalid");
        if (!texture.valid())
            std::runtime_error("Texture is not valid");

        p_texture = texture;
        p_texture.bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_texture.index(), 0);

        p_texture.unbind();
    }

    void attach(const Renderbuffer& renderbuffer)
    {
        if (!binded() && valid())
            std::runtime_error("Framebuffer not binded or invalid");
        if (!renderbuffer.valid())
            std::runtime_error("Renderbuffer is not valid");

        p_renderbuffer = renderbuffer;
        p_renderbuffer.bind();
        p_renderbuffer.storage(p_texture.width(), p_texture.height());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_renderbuffer.index());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer is not complete");

        p_renderbuffer.unbind();
    }

    void create()
    {
        glGenFramebuffers(1, &p_index);
        bind();

        if (!p_texture.valid())
            p_texture.create();
        attach(p_texture);

        if (!p_renderbuffer.valid())
            p_renderbuffer.create();
        attach(p_renderbuffer);

        unbind();
    }

    void rescale(int width, int height)
    {
        p_texture.bind();
        p_texture.rescale(width, height);

        p_renderbuffer.bind();
        p_renderbuffer.storage(p_texture.width(), p_texture.height());

        p_texture.unbind();
        p_renderbuffer.unbind();
    }

    void destroy()
    {
        p_texture.destroy();
        p_renderbuffer.destroy();
        glDeleteFramebuffers(1, &p_index);
    }

    bool valid() const
    {
        return p_index != 0;
    }
};

}