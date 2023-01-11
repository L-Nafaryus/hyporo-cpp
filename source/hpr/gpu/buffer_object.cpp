
#include <glad/glad.h>
#include "buffer_object.hpp"

namespace hpr::gpu
{

    void BufferObject::bind()
    {
        glBindBuffer((GLenum)p_type, p_index);
        p_binded = true;
    }

    void BufferObject::unbind()
    {
        glBindBuffer((GLenum)p_type, 0);
        p_binded = false;
    }

    void BufferObject::destroy()
    {
        if (p_type == Type::Unknown)
            std::runtime_error("Unknown buffer type");

        glDeleteBuffers(1, &p_index);
    }
}
