#pragma once

#include <hpr/containers.hpp>

#include <string>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

namespace hpr::gpu
{

    class Texture
    {

    public:

        using ustring = std::basic_string<unsigned char>;

    public:

        enum class Format
        {
            RGB = GL_RGB,
            RGBA = GL_RGBA
        };

    protected:

        unsigned int p_index;
        std::string p_filename;
        ustring p_source;

        int p_width;
        int p_height;
        Format p_internalFormat;
        Format p_imageFormat;

    public:

        inline
        Texture() :
            p_index {0},
            p_filename {},
            p_source {},
            p_width {},
            p_height {},
            p_internalFormat {Format::RGBA},
            p_imageFormat {Format::RGBA}
        {}

        inline
        Texture(int width, int height) :
            p_index {0},
            p_filename {},
            p_source {},
            p_width {width},
            p_height {height},
            p_internalFormat {Format::RGBA},
            p_imageFormat {Format::RGBA}
        {}

        inline
        Texture(const std::string& filename) :
            p_index {0},
            p_filename {filename},
            p_source {},
            p_width {},
            p_height {},
            p_internalFormat {Format::RGB},
            p_imageFormat {Format::RGB}
        {}

        virtual
        ~Texture() = default;

        [[nodiscard]]
        unsigned int index() const
        {
            return p_index;
        }

        int& width()
        {
            return p_width;
        }

        [[nodiscard]]
        int width() const
        {
            return p_width;
        }

        int& height()
        {
            return p_height;
        }

        [[nodiscard]]
        int height() const
        {
            return p_height;
        }

        void active() const
        {
            glActiveTexture(GL_TEXTURE0 + p_index);
        }

        void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, p_index);
        }

        void unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void alphaChannel(bool enabled)
        {
            if (enabled)
            {
                p_internalFormat = Format::RGBA;
                p_imageFormat = Format::RGBA;
            }
            else
            {
                p_internalFormat = Format::RGB;
                p_imageFormat = Format::RGB;
            }
        }

        void load(const std::string& filename)
        {
            auto filepath = std::filesystem::canonical(std::filesystem::path(filename)).string();

            stbi_set_flip_vertically_on_load(true);
            int channelsCount;
            unsigned char* source = stbi_load(filepath.c_str(), &p_width, &p_height, &channelsCount, 0);

            if (!source)
                throw std::runtime_error("Failed to load texture source");
            else
                create(source);

            stbi_image_free(source);
        }

        void load()
        {
            load(p_filename);
        }

        void create()
        {
            glGenTextures(1, &p_index);
            bind();

            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)p_internalFormat, p_width, p_height, 0, (GLint)p_imageFormat, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            unbind();
        }

        void create(const ustring& source)
        {
            glGenTextures(1, &p_index);
            bind();

            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)p_internalFormat, p_width, p_height, 0, (GLint)p_imageFormat, GL_UNSIGNED_BYTE, source.data());

            p_source = source;

            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            unbind();
        }

        void rescale()
        {
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)p_internalFormat, p_width, p_height, 0, (GLint)p_imageFormat, GL_UNSIGNED_BYTE, !p_source.empty() ? p_source.data() : nullptr);
            unbind();
        }

        void rescale(int width, int height)
        {
            p_width = width;
            p_height = height;
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)p_internalFormat, p_width, p_height, 0, (GLint)p_imageFormat, GL_UNSIGNED_BYTE, !p_source.empty() ? p_source.data() : nullptr);
            unbind();
        }

        void destroy()
        {
            glDeleteTextures(1, &p_index);
        }

        bool valid() const
        {
            return p_index != 0;
        }
    };

}

