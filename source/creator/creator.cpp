#include "../hyporo/gpu/window_system.hpp"
#include "../hyporo/gpu/glfw/window.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main(void)
{
    hpr::gpu::WindowSystem* ws = hpr::gpu::WindowSystem::create(hpr::gpu::WindowContext::Provider::GLFW);
    hpr::gpu::Window* w = ws->newWindow();
    w->init("test", hpr::gpu::Window::Style::Windowed, 0, 0, 600, 400, nullptr, nullptr);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(dynamic_cast<hpr::gpu::glfw::Window*>(w)->instance(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (w->isOpen())
    {
        dynamic_cast<hpr::gpu::glfw::Window*>(w)->pollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool yes = true;
        ImGui::ShowDemoWindow(&yes);

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        {
            if (ImGui::Button("Exit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                w->state(hpr::gpu::Window::State::Closed);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        dynamic_cast<hpr::gpu::glfw::Window*>(w)->swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ws->destroyWindow(w);
    hpr::gpu::WindowSystem::destroy(ws);

    return 0;
}
