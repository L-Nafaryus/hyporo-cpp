#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>


namespace hpr::gpu
{

class Context
{

protected:

    bool p_glInitialized;
    bool p_glfwInitialized;

public:

    inline
    Context() :
        p_glInitialized {false},
        p_glfwInitialized {false}
    {
        if (glfwInit())
            p_glfwInitialized = true;
        else
            throw std::runtime_error("Cannot initialize GLFW context");

    }

    void link()
    {
        if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            p_glInitialized = true;
        else
            throw std::runtime_error("Cannot initialize GLAD context");
    }

    constexpr
    bool valid() const
    {
        return p_glInitialized && p_glfwInitialized;
    }

    inline
    void destroy() const
    {
        glfwTerminate();
    }

    inline
    void debug(bool enable = true)
    {
        const auto debugOutput = [](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
        {
            // ignore non-significant error/warning codes
            if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
                return;

            std::cout << "Debug::GL[" << id << "]::";

            switch (source)
            {
                case GL_DEBUG_SOURCE_API: std::cout << "API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Window_System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader_Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Third_Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Application"; break;
                case GL_DEBUG_SOURCE_OTHER: std::cout << "Other"; break;
                default: break;
            }
            std::cout << "::";

            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR: std::cout << "Error";break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated_Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Undefined_Behaviour"; break;
                case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Performance"; break;
                case GL_DEBUG_TYPE_MARKER: std::cout << "Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Push_Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Pop_Group"; break;
                case GL_DEBUG_TYPE_OTHER: std::cout << "Other"; break;
                default: break;
            }
            std::cout << " ";

            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH: std::cout << "(high)"; break;
                case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "(medium)"; break;
                case GL_DEBUG_SEVERITY_LOW: std::cout << "(low)"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "(notification)"; break;
                default: break;
            }
            std::cout << ": " <<  message << std::endl;
        };

        if (enable)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            glDisable(GL_DEBUG_OUTPUT);
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
    }

};

}
