#pragma once

#include <hpr/math.hpp>
#include <hpr/gpu/shader_program.hpp>

#include "entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Transform
{

public:

    enum Projection
    {
        Perspective,
        Orthographic
    };

protected:

    Projection p_projection;
    hpr::scalar p_distance;
    hpr::scalar p_aspect;
    hpr::scalar p_fieldOfView;
    hpr::scalar p_nearPlane;
    hpr::scalar p_farPlane;

    hpr::vec3 p_target;
    hpr::vec3 p_left;
    hpr::vec3 p_front;
    hpr::vec3 p_up;
    hpr::vec3 p_rotation;

    hpr::mat4 p_model;

    Entity* p_entity;

    hpr::vec3 p_position;

public:

    inline Transform() = delete;

    inline Transform(Entity* entity) :
        p_projection {Orthographic},
        p_distance {10},
        p_aspect {800.f / 600},
        p_fieldOfView {45},
        p_nearPlane {0.1},
        p_farPlane {100},
        p_target {0, 0, 0},
        p_left {1, 0, 0},
        p_front {0, 0, -1},
        p_up {0, 0, 1},
        p_rotation {45, 45, 0},
        p_model {hpr::mat4::identity()},
        p_entity {entity},
        p_position {}
    {
        //for (auto v : p_model)
        //    std::cout << v << std::endl;
    }

    hpr::mat4 view()
    {
        hpr::vec3 rotation {
            hpr::cos(hpr::rad(p_rotation[1])) * hpr::sin(hpr::rad(p_rotation[0])),
            hpr::cos(hpr::rad(p_rotation[1])) * hpr::cos(hpr::rad(p_rotation[0])),
            hpr::sin(hpr::rad(p_rotation[1]))};
        hpr::vec3 pos = p_target + p_distance * rotation;

        p_front = p_target - pos;
        p_front /= hpr::length(p_front);

        p_up = hpr::vec3(0, 0, 1);
        p_left = hpr::normalize(hpr::cross(p_up, p_front));
        p_up = hpr::cross(p_front, p_left);
        auto dd = glm::lookAt(glm::vec3(pos[0], pos[1], pos[2]), glm::vec3(pos[0] + p_front[0], pos[1] + p_front[1], pos[2] + p_front[2]), glm::vec3(p_up[0], p_up[1], p_up[2]));
        auto dd2 = hpr::lookAt(pos, pos + p_front, p_up);
        p_position = pos;
        //for (auto v : dd2)
        //    std::cout << v << " ";
        //std::cout << std::endl;
        return dd2;
    }
/*
    -0.707107   0.707107    0           -0
    -0.5        -0.5        0.707107    -1.90735e-06
    0.5         0.5         0.707107    -10
    0           0           0           1
 */
    void rotateEvent(float xdelta, float ydelta)
    {
        p_rotation += hpr::vec3(xdelta, ydelta, 0);
    }

    void moveEvent(float xdelta, float ydelta)
    {
        p_target += (p_left * xdelta + p_up * ydelta) * 1e-3f;
    }

    void scrollEvent(float xdelta, float ydelta)
    {
        p_distance -= ydelta;
    }

    hpr::mat4 projection(Projection projection)
    {
        p_projection = projection;
        switch (p_projection)
        {
            case Perspective:
                return hpr::perspective(hpr::rad(p_fieldOfView), p_aspect, p_nearPlane, p_farPlane);
            case Orthographic:
                return hpr::ortho(-p_distance * p_aspect, p_distance * p_aspect, -p_distance, p_distance, p_nearPlane, p_farPlane);
        }
    }

    void clear()
    {
        p_model = hpr::mat4::identity();
    }

    void translate(const hpr::vec3& position)
    {
        p_model = hpr::translate(p_model, position);
    }

    void rotate(const hpr::vec3& axis, hpr::scalar angle)
    {
        p_model = hpr::rotate(p_model, axis, hpr::rad(angle));
    }

    void scale(const hpr::vec3& size)
    {
        p_model = hpr::scale(p_model, size);
    }
    /*struct Vec {
        float x, y, z, w;
    };*/
    void render()
    {
        //double model[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        //Vec model2[] = {Vec{0, 0, 0, 0}, Vec{0, 0, 0, 0},Vec{0, 0, 0, 0},Vec{0, 0, 0, 0}};
        //static_assert(sizeof(model2) == sizeof(float) * 16 && std::is_standard_layout_v<Vec>);
        //static_assert((sizeof(hpr::StaticArray<float, 16>::pointer) == (sizeof(GLfloat) * 16)) &&
        //              (std::is_standard_layout<hpr::mat4>::value),
        //              "Matrix4 does not satisfy contiguous storage requirements");
        p_entity->shaderProgram()->bind();

        //glUniformMatrix4fv(glGetUniformLocation(p_entity->shaderProgram()->index(), "model"),
        //                   1, GL_FALSE, p_model.data());
        p_entity->shaderProgram()->uniformMatrix<4, 4, float>("model", 1, true, p_model.data());
        hpr::mat4 _view = view();
        p_entity->shaderProgram()->uniformMatrix<4, 4, float>("view", 1, true, _view.data());
        hpr::mat4 _projection = projection(p_projection);

        p_entity->shaderProgram()->uniformVector<float, 3>("viewPos", 1,  p_position.data());

        //for (auto v : _projection)
        //    std::cout << v << std::endl;
        glm::mat4 _proj = glm::ortho(-p_distance * p_aspect, p_distance * p_aspect, -p_distance, p_distance, p_nearPlane, p_farPlane);

        p_entity->shaderProgram()->uniformMatrix<4, 4, float>("projection", 1, true, _projection.data());

        hpr::vec3 objectColor {1.0f, 0.5f, 0.31f};
        p_entity->shaderProgram()->bind();
        p_entity->shaderProgram()->uniformVector<float, 3>("objectColor", 1,  objectColor.data());
        p_entity->render(gpu::ArrayObject::Mode::Triangles);

        objectColor = {1.0f, 1.0f, 1.0f};
        p_entity->shaderProgram()->bind();
        p_entity->shaderProgram()->uniformVector<float, 3>("objectColor", 1,  objectColor.data());
        p_entity->render(gpu::ArrayObject::Mode::LineLoop);

        p_entity->shaderProgram()->unbind();

        clear();
    }
};