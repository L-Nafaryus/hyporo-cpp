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
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec3 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;
    uniform vec3 overrideColor;
    void main()
    {
        Color = overrideColor * color;
        Texcoord = texcoord;
        gl_Position = proj * view * model * vec4(position, 1.0);
    }
)glsl";
int main()
{
    using namespace hpr;

    gpu::WindowSystem *ws = gpu::WindowSystem::create(gpu::WindowContext::Provider::GLFW);
    gpu::Window *w = ws->newWindow();
    w->init("test", gpu::Window::Style::Windowed, 0, 0, 600, 400, nullptr, nullptr);

    if (gpu::opengl::Device::loadLoader())
        std::cerr << "Load gl loader error" << std::endl;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    /*unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
*/
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

    /*darray<float> data (3 * 6, 0.f);
    auto arr = mesh.face(0)->vertices() + mesh.face(1)->vertices();
    for (auto n = 0; n < arr.size(); ++n)
        for (auto k = 0; k < 3; ++k)
            data[k + 3 * n] = *(arr[n]->data() + k);*/
    darray<unsigned short> indices (6, 0);
    darray<float> data;
    for (auto v : mesh.vertices())
        for (auto c : *v)
            data.push(c);

    indices[0] = 3;
    indices[1] = 1;
    indices[2] = 0;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

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

    /*
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//data.size(), data.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);//indices.size(), indices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices2.size(), indices2.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
*/
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

    gpu::Buffer* vertexBuffer;
    device->createVertexBuffer(&vertexBuffer, sizeof(float) * data.size(), (char*)data.data());
    gpu::Buffer* indexBuffer;
    device->createIndexBuffer(&indexBuffer, sizeof(unsigned short) * indices.size(), (char*)indices.data());


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

        // draw our first triangle
        /*glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
        device->useShaderProgram(shaderProgram);
        device->useVertexBuffer(vertexBuffer, 0, 0);
        device->useIndexBuffer(indexBuffer, 0);
        //dynamic_cast<gpu::opengl::Device*>(device)->Draw(2, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
    delete dynamic_cast<gpu::opengl::Device*>(device);

    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);*/

    ws->destroyWindow(w);
    gpu::WindowSystem::destroy(ws);

    return 0;
}