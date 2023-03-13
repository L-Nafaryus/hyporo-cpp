#pragma once

#include "entity.hpp"


class Grid : public Entity
{

public:

    enum Mode
    {
        XY,
        XZ,
        YZ
    };

protected:

    hpr::scalar p_length;
    hpr::scalar p_width;
    hpr::scalar p_height;
    hpr::scalar p_scale;
    Mode p_mode;

public:


    Grid(gpu::ShaderProgram* shaderProgram, Mode mode = XY, hpr::scalar scale = 1, hpr::scalar length = 100, hpr::scalar width = 100, hpr::scalar height = 100) :
        Entity {shaderProgram},
        p_length {length},
        p_width {width},
        p_height {height},
        p_scale {scale},
        p_mode {mode}
    {
        darray<vec3> vertices;
        darray<vec3> colors;

        construct(vertices, colors);

        Entity::addVertices(vertices);
        Entity::addColors(colors);
        Entity::renderMode(gpu::ArrayObject::Lines);
    }

protected:

    void construct(darray<vec3>& vertices, darray<vec3>& colors)
    {
        const darray<vec3> red {vec3(0.98f, 0.21f, 0.32f), vec3(0.98f, 0.21f, 0.32f)};
        const darray<vec3> green {vec3(0.45f, 0.67f, 0.1f), vec3(0.45f, 0.67f, 0.1f)};
        const darray<vec3> blue {vec3(0.17f, 0.47f, 0.80f), vec3(0.17f, 0.47f, 0.80f)};
        const darray<vec3> white {vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f)};

        switch (p_mode) {
            case XY: {
                // Y
                vertices.push({vec3(0.f, -p_width, 0.f), vec3(0.f, p_width, 0.f)});
                colors.push(green);
                for (hpr::scalar x = p_scale; x <= p_length; )
                {
                    vertices.push({vec3(x, -p_width, 0.f), vec3(x, p_width, 0.f)});
                    colors.push(white);
                    vertices.push({vec3(-x, -p_width, 0.f), vec3(-x, p_width, 0.f)});
                    colors.push(white);
                    x += p_scale;
                }
                // X
                vertices.push({vec3(-p_length, 0.f, 0.f), vec3(p_length, 0.f,  0.f)});
                colors.push(red);
                for (hpr::scalar y = p_scale; y <= p_width; )
                {
                    vertices.push({vec3(-p_length, y, 0.f), vec3(p_length, y, 0.f)});
                    colors.push(white);
                    vertices.push({vec3(-p_length, -y, 0.f), vec3(p_length, -y, 0.f)});
                    colors.push(white);
                    y += p_scale;
                }
                break;
            }
            case XZ: {
                // Z
                vertices.push({vec3(0.f, 0.f, -p_height), vec3(0.f, 0.f, p_height)});
                colors.push(blue);
                for (hpr::scalar x = p_scale; x <= p_length; )
                {
                    vertices.push({vec3(x, 0.f, -p_height), vec3(x, 0.f, p_height)});
                    colors.push(white);
                    vertices.push({vec3(-x, 0.f, -p_height), vec3(-x, 0.f, p_height)});
                    colors.push(white);
                    x += p_scale;
                }
                // X
                vertices.push({vec3(-p_length, 0.f, 0.f), vec3(p_length, 0.f,  0.f)});
                colors.push(red);
                for (hpr::scalar z = p_scale; z <= p_height; )
                {
                    vertices.push({vec3(-p_length, 0.f, z), vec3(p_length, 0.f, z)});
                    colors.push(white);
                    vertices.push({vec3(-p_length, 0.f, -z), vec3(p_length, 0.f, -z)});
                    colors.push(white);
                    z += p_scale;
                }
                break;
            }
            case YZ: {
                // Y
                vertices.push({vec3(0.f, -p_width, 0.f), vec3(0.f, p_width, 0.f)});
                colors.push(green);
                for (hpr::scalar z = p_scale; z <= p_height; )
                {
                    vertices.push({vec3(0.f, -p_width, z), vec3(0.f, p_width, z)});
                    colors.push(white);
                    vertices.push({vec3(0.f, -p_width, -z), vec3(0.f, p_width, -z)});
                    colors.push(white);
                    z += p_scale;
                }
                // Z
                vertices.push({vec3(0.f, 0.f, -p_height), vec3(0.f,  0.f, p_height)});
                colors.push(blue);
                for (hpr::scalar y = p_scale; y <= p_width; )
                {
                    vertices.push({vec3(0.f, y, -p_height), vec3(0.f, y, p_height)});
                    colors.push(white);
                    vertices.push({vec3(0.f, -y, -p_height), vec3(0.f, -y, p_height)});
                    colors.push(white);
                    y += p_scale;
                }
            }
        }
    }

public:

    void rebuild()
    {
        darray<vec3> vertices;
        darray<vec3> colors;

        construct(vertices, colors);

        Entity::editVertices(vertices);
        Entity::editColors(colors);
        Entity::renderMode(gpu::ArrayObject::Lines);
    }

    void mode(Mode mode)
    {
        p_mode = mode;
    }

    void scale(hpr::scalar scale)
    {
        p_scale = scale;
    }
};