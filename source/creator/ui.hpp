#pragma once

#include <hpr/gpu/window.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
//#include <imgui_stdlib.h>


using namespace hpr;

class UI
{


public:

    UI(gpu::Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window->instance(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
        io().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    ~UI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void createFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    inline
    ImGuiIO& io()
    {
        return ImGui::GetIO();
    }

    inline
    void render()
    {
        static auto first_time = true;
        static bool show_object_explorer = true;
        static bool show_properties = true;
        static bool show_demo = false;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::MenuItem("Undo", "CTRL+Z");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Object explorer", nullptr, &show_object_explorer);
                ImGui::MenuItem("Properties", nullptr, &show_properties);
                ImGui::MenuItem("Demo", nullptr, &show_demo);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (show_demo)
            ImGui::ShowDemoWindow();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar; // ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        //window_flags |= ImGuiWindowFlags_MenuBar;
        //window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        if (ImGui::Begin("Main dockspace", nullptr, window_flags))
        {
            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
                //ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 1.0f, &dockspace_id, nullptr);
                ImGuiID dock_id_down = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.5f, nullptr, &dock_id_left);

                ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;

                ImGui::DockBuilderDockWindow("Scene", dockspace_id);
                ImGui::DockBuilderDockWindow("Object explorer", dock_id_left);
                // Properties
                node = ImGui::DockBuilderGetNode(dock_id_down);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;
                ImGui::DockBuilderDockWindow("Properties", dock_id_down);
                //ImGui::DockBuilderDockWindow("Scene_", dock_id_down);
                ImGui::DockBuilderFinish(dockspace_id);
            }

            ImGui::End();
        }

        ImGuiViewport* viewport2 = ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        float height = ImGui::GetFrameHeight();

        if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport2, ImGuiDir_Up, height, window_flags2)) {
            if (ImGui::BeginMenuBar()) {
                ImGui::Text("menu bar");
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        if (ImGui::BeginViewportSideBar("#MainStatusBar", viewport2, ImGuiDir_Down, height, window_flags2)) {
            if (ImGui::BeginMenuBar()) {
                ImGui::Text("status bar");
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }
};