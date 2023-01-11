#pragma once

#include "../math/vector.hpp"
#ifndef __gl_h_
#include <glad/glad.h>
#endif



namespace hpr::gpu
{

    class ColorBuffer
    {

    protected:

        bool p_enabledRed;
        bool p_enabledGreen;
        bool p_enabledBlue;
        bool p_enabledAlpha;
        vec4 p_color;

    public:

        inline
        ColorBuffer() :
            p_enabledRed {true},
            p_enabledGreen {true},
            p_enabledBlue {true},
            p_enabledAlpha {true},
            p_color {}
        {}

        inline
        ColorBuffer(bool red, bool green, bool blue, bool alpha) :
            p_enabledRed {red},
            p_enabledGreen {green},
            p_enabledBlue {blue},
            p_enabledAlpha {alpha},
            p_color {}
        {}

        virtual
        ~ColorBuffer() = default;

        void mask(bool red, bool green, bool blue, bool alpha)
        {
            glColorMask(red, green, blue, alpha);
        }

        inline
        void clear(const vec4& color)
        {
            p_color = color;
            glClearColor(color[0], color[1], color[2], color[3]);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        inline
        void clear()
        {
            clear(p_color);
        }
    };

}

