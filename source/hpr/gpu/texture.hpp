#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Texture : public Context
{
protected:

    std::string p_filename;
    int p_width;
    int p_height;

public:

    // Constructors

    Texture();

    explicit
    Texture(DeviceAPI api);

    ~Texture() override;

    // Member functions

    [[nodiscard]]
    std::string filename() const;

    [[nodiscard]]
    int width() const;

    [[nodiscard]]
    int height() const;
};

} // end namespace hpr::gpu