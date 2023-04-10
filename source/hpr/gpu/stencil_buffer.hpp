#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    class StencilBuffer
    {

    protected:

        bool p_enabled;
        bool p_binded;

    public:

        inline
        StencilBuffer() :
            p_enabled {false},
            p_binded {false}
        {}

        inline
        StencilBuffer(bool enabled) :
            p_enabled {enabled},
            p_binded {false}
        {}

        virtual
        ~StencilBuffer() = default;


        inline
        void bind()
        {
            p_binded = true;
            glEnable(GL_STENCIL_TEST);
        }

        inline
        void unbind()
        {
            p_binded = false;
            glDisable(GL_STENCIL_TEST);
        }

        inline
        bool binded() const
        {
            return p_binded;
        }

        inline
        void enable()
        {
            p_enabled = true;
            glStencilMask(GL_TRUE);
        }

        inline
        void disable()
        {
            p_enabled = false;
            glStencilMask(GL_FALSE);
        }

        [[nodiscard]]
        inline
        bool enabled() const
        {
            return p_enabled;
        }

        inline
        void clear() const
        {
            glClear(GL_STENCIL_BUFFER_BIT);
        }
    };

}
