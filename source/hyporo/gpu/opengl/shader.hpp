
#include "../shader.hpp"


namespace hpr::gpu::opengl
{

class Shader : public gpu::Shader
{
    friend class Device;

protected:

    unsigned int p_shaderIndex;

public:

    // Constructors

    Shader();

    virtual ~Shader();

};

}
