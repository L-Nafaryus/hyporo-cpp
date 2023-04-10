#ifndef __gl_h_
#include <glad/glad.h>
#endif
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

void APIENTRY glDebugOutput(
        GLenum source,
        GLenum type,
        unsigned int id,
        GLenum severity,
        GLsizei length,
        const char* message,
        const void* userParam
)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "Debug::GL[" << id << "]: " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
}
void debug()
{
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

}
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

    //debug();
    gpu::Device* device;
    gpu::Device::create(&device, gpu::Device::DeviceAPI::OpenGL);
    device->initialize();
    gpu::Shader* vertexShader;
    device->createVertexShader(&vertexShader, "shaders/test.vert.glsl", "VS");
    gpu::Shader* fragmentShader;
    device->createFragmentShader(&fragmentShader, "shaders/test.frag.glsl", "FS");
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
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
    //for (auto n = 0; n < arr.size(); ++n)
    //    indices[n] = mesh.indexOf(arr[n]);

    std::cout << "Data: ";
    for (auto p : data)
        std::cout << p << " ";
    std::cout << std::endl;
    std::cout << "Indices: ";
    for (auto p : indices)
        std::cout << p << " ";
    std::cout << std::endl;

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

    gpu::Buffer* vertexBuffer;
    device->createVertexBuffer(&vertexBuffer, sizeof(float) * vertices.size(), (char*)vertices.data());
    gpu::Buffer* indexBuffer;
    device->createIndexBuffer(&indexBuffer, sizeof(unsigned int) * indices2.size(), (char*)indices2.data());

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
        glViewport(0, 0, 600, 400);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        device->useShaderProgram(shaderProgram);
        device->useVertexBuffer(vertexBuffer, 0, 0);
        device->useIndexBuffer(indexBuffer, 0);
        //dynamic_cast<gpu::opengl::Device*>(device)->Draw(2, 0, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
