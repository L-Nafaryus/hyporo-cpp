#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif



namespace hpr::gpu
{

    class Renderbuffer
    {

    protected:

        unsigned int p_index;

    public:

        inline
        Renderbuffer() :
            p_index {0}
        {}

        virtual
        ~Renderbuffer() = default;

        [[nodiscard]]
        unsigned int index() const
        {
            return p_index;
        }

        void bind()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, p_index);
        }

        void unbind()
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void create()
        {
            glGenRenderbuffers(1, &p_index);
            //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_index);
        }

        void storage(int width, int height)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, p_index);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        void destroy()
        {
            glDeleteRenderbuffers(1, &p_index);
        }

        bool valid() const
        {
            return p_index != 0;
        }
    };

}
