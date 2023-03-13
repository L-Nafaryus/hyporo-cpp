#pragma once

#include "camera.hpp"
#include "entity.hpp"

#include <hpr/containers/graph.hpp>


class Scene
{

protected:

    Camera* p_camera;
    hpr::darray<hpr::TreeNode<Entity>> p_nodes;

public:

    inline
    Scene()
    {}

    virtual
    ~Scene()
    {
        delete p_camera;
        for (auto& node : p_nodes)
            node.data()->destroy();
    }

    inline
    void camera(Camera* camera)
    {
        p_camera = camera;
    }

    inline
    Camera* camera()
    {
        return p_camera;
    }

    inline
    void add(const hpr::TreeNode<Entity>& entityNode)
    {
        p_nodes.push(entityNode);
    }

    inline
    darray<hpr::TreeNode<Entity>>& nodes()
    {
        return p_nodes;
    }

    void render()
    {
        for (auto node : p_nodes)
        {
            node.data()->render();

            node.data()->shaderProgram()->bind();
            // camera
            node.data()->shaderProgram()->uniformMatrix<4, 4>("view", 1, true, p_camera->view().data());
            node.data()->shaderProgram()->uniformMatrix<4, 4>("projection", 1, true, p_camera->projection().data());
            node.data()->shaderProgram()->uniformVector<float, 3>("viewPos", 1,  p_camera->position().data());

            // light
            hpr::vec3 lightColor {1.0f, 1.0f, 1.0f};
            node.data()->shaderProgram()->uniformVector<float, 3>("lightColor", 1,  lightColor.data());
            hpr::vec3 lightPos {1.0f, 1.0f, 1.0f};
            node.data()->shaderProgram()->uniformVector<float, 3>("lightPos", 1,  lightPos.data());

            node.data()->shaderProgram()->unbind();

            //for (auto descendant : node.descendants())
            //    descendant->data()->render();
        }
    }

};
