
#include <iostream>

#include "visual.hpp"
#include "entity.hpp"
#include "ui.hpp"
//#include "transform.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "grid.hpp"
#include <implot.h>
#include "ray.hpp"


int main()
{

    Visual visual;

    visual.window()->keyClickCallback([](gpu::Window* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            window->state(gpu::Window::Closed);
    });

    visual.window()->context().debug();

    UI ui {visual.window()};


    darray<vec3> vertices {
            /*vec2(-0.5f,  0.5f),
            vec2(0.5f,  0.5f),
            vec2(0.5f, -0.5f),
            vec2(-0.5f, -0.5f)*/
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
             vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(-0.5f,  0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),

            vec3(-0.5f, -0.5f,  0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f, -0.5f,  0.5f),

            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(-0.5f, -0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),

            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),

            vec3(-0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(-0.5f, -0.5f,  0.5f),
            vec3(-0.5f, -0.5f, -0.5f),

            vec3(-0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f, -0.5f)
    };
    darray<vec3> normals {
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),

            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),

            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),

            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),

            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),

            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f)
    };
    darray<vec3> colors {
            vec3(1.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f),
            vec3(0.0f, 0.0f, 1.0f),
            vec3(1.0f, 1.0f, 1.0f)
    };
    darray<Vector<unsigned int, 3>> indices {
        Vector<unsigned int, 3>(0, 1, 2),
        Vector<unsigned int, 3>(2, 3, 0)
    };

    Entity entity {visual.shaderProgram()};
    entity.addVertices(vertices);
    entity.addNormals(normals);
    //entity.addColors(colors);
    //entity.addIndices(indices);

    Scene scene;
    scene.camera(new OrbitCamera());
    scene.add(TreeNode<Entity>(entity));
    //Transform transform {&entity};

    //gpu::DepthBuffer depthBuffer {true};
    visual.depthBuffer().bind();
    //gpu::CullFace cullFace {gpu::CullFace::Mode::FrontAndBack};
    float angle = 0;
    glEnable(GL_PROGRAM_POINT_SIZE);

    /*gpu::ShaderProgram shaderProgram;
    gpu::Shader vertexShader {gpu::Shader::Type::Vertex, gridVertexSource};
    vertexShader.create();

    gpu::Shader fragmentShader {gpu::Shader::Type::Fragment, gridFragmentSource};
    fragmentShader.create();

    shaderProgram.create();
    shaderProgram.attach(vertexShader);
    shaderProgram.attach(fragmentShader);
    shaderProgram.link();

    shaderProgram.bind();
    shaderProgram.uniformMatrix<4, 4>("view", 1, true, scene.camera()->view().data());
    shaderProgram.uniformMatrix<4, 4>("projection", 1, true, scene.camera()->projection().data());
    shaderProgram.unbind();*/

    /*Entity grid {visual.shaderProgram()};

    darray<vec3> vertices2;
    auto genVert = [](float x, float y){;
        return darray<vec3>{
        vec3(-1.0f + x, 0.f + y, 0.f),
        vec3(1.0f + x, 0.f + y, 0.f),
        vec3(0.f + x,  -1.0f + y, 0.f),
        vec3(0.f + x,  1.0f + y, 0.f)
    };};
    for (auto x = -100; x < 100; ++x)
        for (auto y = -100; y < 100; ++y)
            vertices2.push(genVert(x, y));
    grid.addVertices(vertices2);
    grid.color(vec4(1, 1, 1, 1));
    grid.renderMode(gpu::ArrayObject::Lines);*/
    //Grid grid {visual.shaderProgram()};
    //scene.add(TreeNode<Entity>(grid));
    /*
    gpu::ArrayObject ao;
    gpu::BufferObject bo;
    ao.create();
    ao.bind();
    bo.create(vertices2);
    bo.bind();
    ao.attribPointer(bo, 0, 3);
    ao.unbind();
    bo.unbind();*/
    glfwWindowHint(GLFW_SAMPLES, 4);
    ImPlot::CreateContext();
    glEnable(GL_MULTISAMPLE);
    /*gpu::Texture tex;
    tex.create();
    gpu::Renderbuffer rb;
    rb.create();*/
    gpu::Framebuffer framebuffer;
    framebuffer.create();
    gpu::Viewport viewport;
    /*framebuffer.bind();
    framebuffer.attach(tex);
    framebuffer.attach(rb);
    framebuffer.unbind();*/
    const vec4 background = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    while (visual.window()->state() != gpu::Window::Closed)
    {

        visual.colorBuffer().clear(background);
        visual.depthBuffer().clear();
        //cullFace.bind();
        //entity.render(hpr::gpu::ArrayObject::Triangles);
        //angle += 1;
        //transform.rotate({0, 1, 0}, angle);
        //transform.rotate({0, 0, 1}, angle);
        //transform.render();
        viewport.size() = vec2(framebuffer.width(), framebuffer.height());
        viewport.set();
        framebuffer.bind();
        framebuffer.rescale();

        visual.colorBuffer().clear(background);
        visual.depthBuffer().clear();

        scene.camera()->aspect() = (float)framebuffer.width() / (float)framebuffer.height();
        scene.render();
        framebuffer.unbind();
        viewport.size() = vec2((float)visual.window()->width(), (float)visual.window()->height());
        viewport.set();
        //shaderProgram.bind();

        ui.createFrame();
        bool yes = true;
        ImGui::ShowDemoWindow(&yes);
        ImPlot::ShowDemoWindow(&yes);

        ImGui::Begin("Hello, world!");
        {

            if (ImGui::Button("Exit"))
                visual.window()->state(gpu::Window::Closed);
            ImGui::End();
        }

        ui.render();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::Begin("Scene"))
        {
            ImGui::PopStyleVar();
            if (!ImGui::IsWindowCollapsed())
            {
                framebuffer.width() = static_cast<int>(ImGui::GetContentRegionAvail().x);
                framebuffer.height() = static_cast<int>(ImGui::GetContentRegionAvail().y);

                ImGui::Image(reinterpret_cast<void *>(framebuffer.texture().index()), ImGui::GetContentRegionAvail(),
                             ImVec2{0, 1}, ImVec2{1, 0});

                ImGuiIO& io = ImGui::GetIO();
                Ray ray;
                ray.fromScreen(io.MousePos.x, io.MousePos.y, framebuffer.width(), framebuffer.height(), scene.camera());
                float tMin, tMax;
                Entity* ent = scene.nodes()[0].data();
                bool rayOBB = ray.obb({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, ent, tMin, tMax);
                //std::cout << rayOBB << std::endl;
                if (rayOBB)
                    ent->color(vec4(1.f, 0.f, 0.f, 1.f));
                else
                    ent->color(vec4(0.f, 1.f, 0.f, 1.f));
                if (ImGui::Begin("Properties"))
                {
                    ImGui::DragFloat3("position", ray.p_position.data());
                    ImGui::DragFloat3("direction", ray.p_direction.data());
                    vec2 mouse {io.MousePos.x, io.MousePos.y};
                    ImGui::DragFloat2("mouse", mouse.data());
                    ImGui::Checkbox("ray", &rayOBB);
                    ImGui::DragFloat("dist", &tMin);
                    ImGui::DragFloat("dist", &tMax);
                    ImGui::End();
                }
                if (ImGui::IsWindowHovered())
                {
                    if (io.WantCaptureMouse)
                    {
                        dynamic_cast<OrbitCamera*>(scene.camera())->scrollEvent(0, io.MouseWheel);

                        if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) //(io.MouseDown[ImGuiMouseButton_Middle])
                        {
                            if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) //(io.KeysDown[ImGuiKey_LeftShift])
                            {
                                dynamic_cast<OrbitCamera*>(scene.camera())->moveEvent(io.MouseDelta.x, io.MouseDelta.y);
                            }
                            else
                            {
                                dynamic_cast<OrbitCamera*>(scene.camera())->rotateEvent(io.MouseDelta.x, io.MouseDelta.y);
                            }
                        }

                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                        {


                        }
                    }
                }
            }
            ImGui::End();
        }
        ui.renderFrame();
        visual.render();

    }
    ImPlot::DestroyContext();
    framebuffer.destroy();
    return 0;
}
