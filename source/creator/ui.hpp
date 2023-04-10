#pragma once

#include <hpr/gpu/window.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui_freetype.h>

//#include <imgui_stdlib.h>
#include "FontAwesome6.hpp"

#include <implot.h>

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

        ImPlot::CreateContext();

        configure();
    }

    ~UI()
    {
        ImPlot::DestroyContext();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void configure()
    {
        auto& io = ImGui::GetIO();

        // features
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // fonts
        //io.Fonts->AddFontDefault();
        float baseFontSize = 16.0f; // 13.0f is the size of the default font. Change to the font size you use.
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;

        io.Fonts->AddFontFromFileTTF("droidsans.ttf", 14.f);
        io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges );
        io.Fonts->Build();

        // style
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
        style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
        style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
        style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
        style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
        style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
        style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
        style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.GrabRounding                           = style.FrameRounding = 2.3f;
        style.WindowRounding = 10.f;
        style.WindowPadding = ImVec2(2.f, 2.f);
        style.ScrollbarSize = 12.f;
    }


    void createFrame()
    {
        float           RasterizerMultiply = 1.0f;
        unsigned int    FreeTypeBuilderFlags = 0;

        ImFontAtlas* atlas = ImGui::GetIO().Fonts;
        for (int n = 0; n < atlas->ConfigData.Size; n++)
            ((ImFontConfig*)&atlas->ConfigData[n])->RasterizerMultiply = RasterizerMultiply;

        atlas->FontBuilderIO = ImGuiFreeType::GetBuilderForFreeType();
        atlas->FontBuilderFlags = FreeTypeBuilderFlags;

        /*
        atlas->FontBuilderIO = ImFontAtlasGetBuilderForStbTruetype();
        atlas->FontBuilderFlags = 0;
        */

        atlas->Build();
        ImGui_ImplOpenGL3_DestroyDeviceObjects();
        ImGui_ImplOpenGL3_CreateDeviceObjects();

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

    bool menuFileNew;
    bool menuFileOpen;
    bool menuClose;
    bool helpImguiDemo;
    bool helpImplotDemo;

    void mainMenu()
    {
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.12f, 0.12f, 0.12f, 1.f));

        if (ImGui::BeginMainMenuBar())
        {
            ImGui::PopStyleColor(1);

            if (ImGui::BeginMenu("File"))
            {
                menuFileNew = ImGui::MenuItem(ICON_FA_FILE " New");
                menuFileOpen = ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open...");
                ImGui::Separator();
                menuClose = ImGui::MenuItem(ICON_FA_POWER_OFF " Quit");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::MenuItem("Undo", "Ctrl Z");
                ImGui::MenuItem("Redo", "Shift Ctrl Z");
                ImGui::Separator();
                ImGui::MenuItem(ICON_FA_GEAR " Preferences...");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem(ICON_FA_GLOBE " Documentation");
                ImGui::MenuItem(ICON_FA_GLOBE " Issues");
                ImGui::Separator();
                ImGui::MenuItem("ImGui demo", nullptr, &helpImguiDemo);
                ImGui::MenuItem("ImPlot demo", nullptr, &helpImplotDemo);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (helpImguiDemo)
            ImGui::ShowDemoWindow();

        if (helpImplotDemo)
            ImPlot::ShowDemoWindow();
    }

    void mainDockspace()
    {
        auto split = [](ImGuiID& parentNode, ImGuiDir direction, unsigned int currentID, unsigned int lastID)
        {
            //ImGui::DockBuilderSetNodePos(parent_node, ImGui::GetWindowPos());
            //ImGui::DockBuilderSetNodeSize(parent_node, ImGui::GetWindowSize());
            ImGuiID nodeA;
            ImGuiID nodeB;
            ImGui::DockBuilderSplitNode(parentNode, direction, 0.5f, &nodeB, &nodeA);

            ImGui::DockBuilderDockWindow((std::string("WindowDock#") + std::to_string(currentID)).data(), nodeA);
            ImGui::DockBuilderDockWindow((std::string("WindowDock#") + std::to_string(lastID + 1)).data(), nodeB);

            ImGuiDockNode* node = ImGui::DockBuilderGetNode(nodeB);
            node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

            ImGui::DockBuilderFinish(parentNode);
            parentNode = nodeA;

            return nodeB;
        };
        static auto first_time = true;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar; // ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        //window_flags |= ImGuiWindowFlags_MenuBar;
        //window_flags |= ImGuiWindowFlags_NoBackground;

        //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        if (ImGui::Begin("#Dockspace", nullptr, window_flags))
        {
            ImGui::PopStyleVar(2);

            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            ImGuiID dockspace_id = ImGui::GetID("Dockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                // [ 1.1 ] [ 2.1 ]
                // [     ] [ 2.2 ]
                ImGuiID dock21 = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
                ImGuiID dock22 = ImGui::DockBuilderSplitNode(dock21, ImGuiDir_Down, 0.5f, nullptr, &dock21);

                ImGui::DockBuilderDockWindow("Component#1", dockspace_id);
                ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar ;

                ImGui::DockBuilderDockWindow("Component#2", dock21);
                node = ImGui::DockBuilderGetNode(dock21);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

                ImGui::DockBuilderDockWindow("Component#3", dock22);
                node = ImGui::DockBuilderGetNode(dock22);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

                ImGui::DockBuilderFinish(dockspace_id);
            }

            ImGui::End();
        }
    }



    inline
    void statusBar()
    {
        if (ImGui::BeginViewportSideBar("#MainStatusBar", ImGui::GetMainViewport(), ImGuiDir_Down, ImGui::GetFrameHeight(), ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar()) {
                ImGui::Text("status bar");
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }

};