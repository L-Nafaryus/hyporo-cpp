#include "hpr/window_system/window_system.hpp"
#include "hpr/window_system/glfw/window_system.hpp"
#include "hpr/window_system/glfw/window.hpp"
#include "hpr/gpu.hpp"
#include "hpr/math.hpp"
#include "hpr/mesh.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

struct ConstantBuffer {
    hpr::mat4 World;
    hpr::mat4 View;
    hpr::mat4 Projection;

    hpr::vec4 Eye;

    float Color[4];
    float Offset[4];

    float Scale;
    int AllWhiteLight;

    int Pad[2];
};

struct ObjectBuffer {
    hpr::mat4 ObjectTransform;

    hpr::vec4 DiffuseColor;

    hpr::vec4 FalloffColor;
    float FalloffPower;

    int UseDiffuseMap;
    int UseSpecularMap;
    int UseNormalMap;
    int UseFalloff;
    int UseAlpha;
    int UseReflectMap;

    float Specular;
    float SpecularPower;
    float ReflectPower;

    float Ambient;

    int Lit;
    int LightExclusion[3];

    int Pad[1];
};

int main()
{
    using namespace hpr;

    gpu::WindowSystem* ws = gpu::WindowSystem::create(gpu::WindowContext::Provider::GLFW);
    gpu::Window* w = ws->newWindow();
    w->init("test", gpu::Window::Style::Windowed, 0, 0, 600, 400, nullptr, nullptr);

    if (gpu::opengl::Device::loadLoader())
        std::cerr << "Load gl loader error" << std::endl;

    gpu::Device* device;
    gpu::Device::create(&device, gpu::Device::DeviceAPI::OpenGL);
    device->initialize();
    gpu::Shader* vertexShader;
    device->createVertexShader(&vertexShader, "shaders/base.vert.glsl", "VS");
    gpu::Shader* fragmentShader;
    device->createFragmentShader(&fragmentShader, "shaders/base.frag.glsl", "FS");
    gpu::ShaderProgram* shaderProgram;
    device->createShaderProgram(&shaderProgram);
    device->attachShader(shaderProgram, vertexShader);
    device->attachShader(shaderProgram, fragmentShader);
    device->linkProgram(shaderProgram);

    mesh::Mesh mesh;
    mesh.addVertex(-1, 1, 0.5);
    mesh.addVertex(1, 1, 0.5);
    mesh.addVertex(1, -1, 0.5);
    mesh.addVertex(-1, -1, 0.5);
    mesh.addEdge(mesh.vertex(0), mesh.vertex(1));
    mesh.addEdge(mesh.vertex(1), mesh.vertex(2));
    mesh.addEdge(mesh.vertex(2), mesh.vertex(3));
    mesh.addEdge(mesh.vertex(3), mesh.vertex(0));
    mesh.addEdge(mesh.vertex(0), mesh.vertex(2));
    mesh.addFace(mesh.edge(0), mesh.edge(1), mesh.edge(4));
    mesh.addFace(mesh.edge(2), mesh.edge(3), mesh.edge(4));

    darray<float> data (3 * 6, 0.f);
    auto arr = mesh.face(0)->vertices() + mesh.face(1)->vertices();
    for (auto n = 0; n < arr.size(); ++n)
        for (auto k = 0; k < 3; ++k)
            data[k + 3 * n] = *(arr[n]->data() + k);
    darray<unsigned short> indices (6, 0);
    for (auto n = 0; n < arr.size(); ++n)
        indices[n] = mesh.indexOf(arr[n]);

    gpu::Buffer* vertexBuffer;
    device->createVertexBuffer(&vertexBuffer, sizeof(float) * data.size(), (char*)data.data());
    gpu::Buffer* indexBuffer;
    device->createIndexBuffer(&indexBuffer, sizeof(unsigned short) * indices.size(), (char*)indices.data());

    gpu::Buffer* constantBuffer;
    device->createUniformBuffer(&constantBuffer, sizeof(ConstantBuffer), nullptr);
    gpu::Buffer* objectBuffer;
    device->createUniformBuffer(&objectBuffer, sizeof(ObjectBuffer), nullptr);

    ConstantBuffer constantData;
    constantData.Color[0] = 1;
    constantData.Color[1] = 0;
    constantData.Color[2] = 0;
    constantData.Color[3] = 1;
    constantData.Offset[0] = 0;
    constantData.Offset[1] = 0;
    constantData.Offset[2] = 0;
    constantData.Offset[3] = 1;
    constantData.Scale = 1;
    ObjectBuffer objectData;
    objectData.Lit = 1;
    objectData.DiffuseColor = vec4(1, 1, 1, 1);
    objectData.Specular = 0.25;
    objectData.SpecularPower = 1;
    objectData.UseDiffuseMap = 0;
    objectData.UseSpecularMap = 0;
    objectData.UseNormalMap = 0;
    objectData.UseFalloff = 0;
    objectData.FalloffColor = vec4(1, 1, 1, 1);
    objectData.FalloffPower = 1;
    objectData.Ambient = 1;
    objectData.UseAlpha = 0;
    objectData.UseReflectMap = 0;

    device->useShaderProgram(shaderProgram);
    device->useUniformBuffer(constantBuffer, 0);
    device->useUniformBuffer(objectBuffer, 1);

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
        dynamic_cast<gpu::glfw::Window*>(w)->pollEvents();
        device->useShaderProgram(shaderProgram);
        device->useVertexBuffer(vertexBuffer, 0, 0);
        device->useIndexBuffer(indexBuffer, 0);
        dynamic_cast<gpu::opengl::Device*>(device)->Draw(2, 0, 0);
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
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    device->destroyShaderProgram(shaderProgram, false);
    device->destroyShader(vertexShader);
    device->destroyShader(fragmentShader);
    device->destroyBuffer(vertexBuffer);
    device->destroyBuffer(indexBuffer);
    device->destroyBuffer(constantBuffer);
    device->destroyBuffer(objectBuffer);
    delete dynamic_cast<gpu::opengl::Device*>(device);

    ws->destroyWindow(w);
    gpu::WindowSystem::destroy(ws);

    return 0;
}
