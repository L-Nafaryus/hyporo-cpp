
#include "hpr/gpu.hpp"
#include "hpr/window_system/window_system.hpp"
#include "hpr/window_system/glfw/window_system.hpp"
#include "hpr/window_system/glfw/window.hpp"
#include "hpr/math.hpp"
#include "hpr/mesh.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main()
{
    using namespace hpr;

    gpu::WindowSystem *ws = gpu::WindowSystem::create(gpu::WindowContext::Provider::GLFW);
    gpu::Window *w = ws->newWindow();
    w->init("test", gpu::Window::Style::Windowed, 0, 0, 600, 400, nullptr, nullptr);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw std::runtime_error("Cannot initialize gl context");

    gpu::Shader vshader {gpu::Shader::Type::Vertex, vertexShaderSource};
    gpu::Shader fshader {gpu::Shader::Type::Fragment, fragmentShaderSource};
    gpu::ShaderProgram sprogram {};

    vshader.create();
    fshader.create();
    sprogram.create();
    sprogram.attach(vshader);
    sprogram.attach(fshader);
    sprogram.link();
    for (auto& sh : sprogram.shaders())
        std::cout << sh.index() << std::endl;


    darray<float> vertices {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    darray<unsigned int> indices2 {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

    gpu::ArrayObject vao {};
    gpu::BufferObject vbo {gpu::BufferObject::Type::Vertex};
    gpu::BufferObject ebo {gpu::BufferObject::Type::Index};

    vao.create();
    vao.bind();
    vbo.create<float>(vertices);
    ebo.create<unsigned int>(indices2);
    vao.attribPointer(vbo, 0, 3);
    vao.unbind();

    gpu::Viewport viewport {{0, 0}, {600, 400}};
    gpu::ColorBuffer colorBuffer;
    gpu::DepthBuffer depthBuffer {true};

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(dynamic_cast<gpu::glfw::Window*>(w)->instance(), true);
    ImGui_ImplOpenGL3_Init("#version 420");

    while (w->isOpen())
    {
        viewport.set();

        colorBuffer.clear({0.8f, 0.2f, 0.2f, 1.0f});
        depthBuffer.clear();

        sprogram.bind();
        vao.bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool yes = true;
        ImGui::ShowDemoWindow(&yes);

        ImGui::Begin("Hello, world!");
        {
            if (ImGui::Button("Exit"))
                w->state(gpu::Window::State::Closed);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        dynamic_cast<gpu::glfw::Window*>(w)->swapBuffers();
        dynamic_cast<gpu::glfw::Window*>(w)->pollEvents();
    }

    sprogram.destroy();
    vshader.destroy();
    fshader.destroy();
    vbo.destroy();
    ebo.destroy();
    vao.destroy();

    ws->destroyWindow(w);
    gpu::WindowSystem::destroy(ws);

    return 0;
}