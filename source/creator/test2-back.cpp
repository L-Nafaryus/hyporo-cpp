

/*
#include "hpr/gpu.hpp"
#include "hpr/window_system/window_system.hpp"
#include "hpr/window_system/glfw/window_system.hpp"
#include "hpr/window_system/glfw/window.hpp"
#include "hpr/math.hpp"
#include "hpr/mesh.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
}
*/

/*
const char *vertexShaderSource = "#version 430 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 430 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";
int main()
{
    using namespace hpr;

    gpu::WindowSystem *ws = gpu::WindowSystem::create(gpu::WindowContext::Provider::GLFW);
    gpu::Window *w = ws->newWindow();
    w->init("test", gpu::Window::Style::Windowed, 50, 50, 600, 400, nullptr, nullptr);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw std::runtime_error("Cannot initialize gl context");

// During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback( glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    gpu::Shader vshader {gpu::Shader::Type::Vertex, vertexShaderSource};
    gpu::Shader fshader {gpu::Shader::Type::Fragment, fragmentShaderSource};
    gpu::ShaderProgram sprogram {};

    vshader.create();
    fshader.create();
    sprogram.create();
    sprogram.attach(vshader);
    sprogram.attach(fshader);
    sprogram.link();

gpu::Shader vshader2 {gpu::Shader::Type::Vertex, vertexSource};
    gpu::Shader fshader2 {gpu::Shader::Type::Fragment, fragmentSource};
    gpu::ShaderProgram sprogram2 {};

    vshader2.create();
    fshader2.create();
    sprogram2.create();
    sprogram2.attach(vshader2);
    sprogram2.attach(fshader2);
    sprogram2.link();

    //vshader.destroy();
    //fshader.destroy();
    for (auto& sh : sprogram.shaders())
        std::cout << sh.index() << std::endl;
    std::cout << sprogram.index() << std::endl;

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

    // Create Vertex Array Object
    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo2;
    glGenBuffers(1, &vbo2);

    GLfloat vertices2[] = {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices2, GL_STATIC_DRAW);

    // Create an element array
    GLuint ebo2;
    glGenBuffers(1, &ebo2);

    GLuint elements2[] = {
            0, 1, 2,
            2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(sprogram2.index(), "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(sprogram2.index(), "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    std::cout << sprogram2.index() << std::endl;
    gpu::Viewport viewport {{0, 0}, {600, 400}};
    gpu::ColorBuffer colorBuffer;
    gpu::DepthBuffer depthBuffer {true};

    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(dynamic_cast<gpu::glfw::Window*>(w)->instance(), true);
    ImGui_ImplOpenGL3_Init("#version 430");

    while (w->isOpen())
    {
        viewport.set();

        //colorBuffer.clear({0.2f, 0.2f, 0.2f, 1.0f});
        //depthBuffer.clear();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprogram.bind();
        vao.bind();
        ebo.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        sprogram2.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
/*
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

    vbo.destroy();
    ebo.destroy();
    vao.destroy();
/*
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ws->destroyWindow(w);
    gpu::WindowSystem::destroy(ws);

    return 0;
}
*/


// Link statically with GLEW
#define GLEW_STATIC

// Headers
//#include <GL/glew.h>
//#include <SFML/Window.hpp>

#include "hpr/gpu.hpp"

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

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 430 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

    //in vec2 position;
    //in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 430 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        //outColor = vec4(1.0, 1.0, 1.0, 1.0); //vec4(Color, 1.0);
        outColor = vec4(Color, 1.0);
    }
)glsl";

int main()
{
    /*sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
*/

    using namespace hpr;
//gpu::WindowSystem *ws = gpu::WindowSystem::create(gpu::WindowContext::Provider::GLFW);
    //gpu::Window *w = ws->newWindow();
    //w->init("test", gpu::Window::Style::Windowed, 50, 50, 600, 400, nullptr, nullptr);
    gpu::Window w {50, 50, 600, 400, "Test", gpu::Window::Style::Windowed, nullptr, nullptr};

    w.keyClickCallback([](gpu::Window* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            window->state(gpu::Window::Closed);
    });

    w.context().debug();
    // Initialize GLEW
    //glewExperimental = GL_TRUE;
    //glewInit();

    // Create Vertex Array Object
    /*GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);*/

    gpu::ArrayObject vao {};
    vao.create();
    vao.bind();

    // Create a Vertex Buffer Object and copy the vertex data to it
    //GLuint vbo;
    //glGenBuffers(1, &vbo);
    gpu::BufferObject vbo2 {gpu::BufferObject::Type::Vertex};

    darray<float> vertices {
            -0.5f,  0.5f, //1.0f, 0.0f, 0.0f, // Top-left
            0.5f,  0.5f, //0.0f, 1.0f, 0.0f, // Top-right
            0.5f, -0.5f, //0.0f, 0.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, //1.0f, 1.0f, 1.0f  // Bottom-left
    };
    darray<float> colors {
            1.0f, 0.0f, 0.0f, // Top-left
            0.0f, 1.0f, 0.0f, // Top-right
            0.0f, 0.0f, 1.0f, // Bottom-right
            1.0f, 1.0f, 1.0f  // Bottom-left
    };
    float vertices2[] =  {
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo.index());
    //glBufferData(static_cast<GLenum>(gpu::BufferObject::Type::Vertex), sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    //vbo.bind();
    vbo2.create(vertices);
    vbo2.bind();

    gpu::BufferObject vboColors {gpu::BufferObject::Type::Vertex};
    vboColors.create(colors);
    vboColors.bind();
    //vbo2.destroy();

    // Create an element array
    /*GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
    };*/
    darray<unsigned int> elements  {
            0, 1, 2,
            2, 3, 0
    };
/*
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
*/
    gpu::BufferObject els {gpu::BufferObject::Type::Index};
    els.create(elements);
    els.bind();
    // Create and compile the vertex shader
    /*GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);*/

    gpu::Shader vshader {gpu::Shader::Type::Vertex, vertexSource};
    vshader.create();

    // Create and compile the fragment shader
    /*GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);*/

    gpu::Shader fshader {gpu::Shader::Type::Fragment, fragmentSource};
    fshader.create();

    // Link the vertex and fragment shader into a shader program
    /*GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);*/

    gpu::ShaderProgram sprogram {};
    sprogram.create();
    sprogram.attach(vshader);
    sprogram.attach(fshader);
    sprogram.link();
    sprogram.bind();

    // Specify the layout of the vertex data
    /*GLint posAttrib = glGetAttribLocation(sprogram.index(), "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);*/
    vao.attribPointer(vbo2, 0, 2);
    vao.attribPointer(vboColors, 1, 3);
    /*vbo2.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    vboColors.bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);*/
    /*GLint colAttrib = glGetAttribLocation(sprogram.index(), "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));*/

    gpu::ColorBuffer cb;

    bool running = true;
    while (w.state() != gpu::Window::Closed)
    {
        /*sf::Event windowEvent;
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
                case sf::Event::Closed:
                    running = false;
                    break;
            }
        }*/

        // Clear the screen to black
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        cb.clear(vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // Draw a rectangle from the 2 triangles using 6 indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Swap buffers
        //window.display();
        w.swapBuffers();
        w.pollEvents();
    }

    sprogram.destroy();
    vshader.destroy();
    fshader.destroy();
    /*glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);*/

    //glDeleteBuffers(1, &ebo);
    //glDeleteBuffers(1, &vbo);
    vbo2.destroy();

    //glDeleteVertexArrays(1, &vao);
    vao.destroy();
    w.destroy();

    return 0;
}
