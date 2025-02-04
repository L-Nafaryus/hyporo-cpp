#pragma once

#include <hpr/gpu.hpp>
#include <hpr/math.hpp>

#include "drawable.hpp"


class Entity : public Drawable
{

protected:

    hpr::vec3 p_position;
    hpr::quat p_rotation;
    hpr::vec3 p_scale;
    std::string p_label;

public:

    bool selected;

    Entity() :
        Drawable {}
    {}

    inline
    Entity(const Entity& entity) = default;
    /*{
        std::cout << "copy" << std::endl;
    }*/

    ~Entity() override = default;

    inline virtual
    void rotation(const hpr::vec3& angles)
    {
        p_rotation = hpr::quat(hpr::quat::XYZ, angles);
    }

    inline virtual
    hpr::quat& rotation()
    {
        return p_rotation;
    }

    inline virtual
    void rotate(const hpr::vec3& axis, hpr::scalar angle)
    {
        p_rotation *= hpr::quat(axis, angle);
    }

    inline virtual
    std::string label() const
    {
        return p_label;
    }

    inline virtual
    void label(const std::string& label)
    {
        p_label = label;
    }

    hpr::mat4 model()
    {
        hpr::mat4 model = hpr::mat4::identity();
        hpr::translate(model, p_position);
        hpr::rotate(model, p_rotation);
        hpr::scale(model, p_scale);
        return model;
    }

    inline
    void render(gpu::ShaderProgram* shaderProgram) override
    {
        shaderProgram->bind();
        //std::cout << shaderProgram()->index() << std::endl;

        //for (auto v : model)
        //    std::cout << v << " ";
        //std::cout << std::endl;
        shaderProgram->uniformMatrix<4, 4>("model", 1, true, model().data());

        Drawable::render(shaderProgram);

        shaderProgram->unbind();
    }

};