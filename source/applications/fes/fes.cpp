#include <iostream>
#include <OpenXLSX.hpp>
#include <hpr/gpu.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <implot.h>
#include <string>
#include <hpr/containers.hpp>

int main()
{
    OpenXLSX::XLDocument doc;
    doc.open("fes.xlsx");
    auto wb = doc.workbook();
    auto wks = wb.worksheet("Капилляриметрия");
    std::cout << wb.sheetCount() << std::endl;
    auto rng = wks.range(OpenXLSX::XLCellReference("B7"), OpenXLSX::XLCellReference("B52"));
    //for (auto cell : rng)
    //    std::cout << cell.value() << std::endl;
    //xlnt::workbook wb;
    //wb.load("fes.xlsx");
    //auto ws = wb.active_sheet();


    //std::clog << wb.sheet_count() << std::endl;
    auto window = hpr::gpu::Window{50, 50, 1000, 800, "FES", hpr::gpu::Window::Windowed, nullptr, nullptr};

    window.keyClickCallback([](hpr::gpu::Window* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            window->state(hpr::gpu::Window::Closed);
    });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window.instance(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    while (window.state() != hpr::gpu::Window::Closed)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static int selected = 0;

        if (ImGui::Begin("Table"))
        {
            if (ImGui::BeginTable("##table", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_BordersOuterV))
            {
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Core sample", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("Depth", ImGuiTableColumnFlags_NoHide);
                ImGui::TableSetupColumn("Lithotype", ImGuiTableColumnFlags_NoHide);
                ImGui::TableHeadersRow();
                const int start = 7;//54;
                const int end = 11;//113;
                int n = start;

                auto sample_range = wks.range(OpenXLSX::XLCellReference("B" + std::to_string(7)), OpenXLSX::XLCellReference("B" + std::to_string(11)));
                hpr::darray<OpenXLSX::XLCell> sample {sample_range.begin(), sample_range.end()};
                //auto depth_range = wks.range(OpenXLSX::XLCellReference("H" + std::to_string(start)), OpenXLSX::XLCellReference("H" + std::to_string(end)));
                //hpr::darray<OpenXLSX::XLCell> depth {sample_range.begin(), sample_range.end()};
                //auto lithotype_range = wks.range(OpenXLSX::XLCellReference("K" + std::to_string(start)), OpenXLSX::XLCellReference("K" + std::to_string(end)));
                //hpr::darray<OpenXLSX::XLCell> lithotype {sample_range.begin(), sample_range.end()};

                for ( ; n < end; ++n)
                {

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::TextDisabled("#%d", n);

                    ImGui::TableNextColumn();
                    //float value = static_cast<float>(cell.value());
                    if (ImGui::Selectable(sample[n - start].value().get<std::string>().c_str()))
                    {
                        selected = n;
                    }

                    ImGui::TableNextColumn();
                    //ImGui::Text(depth[n - start].value().get<std::string>().c_str());
                    ImGui::TableNextColumn();
                    //ImGui::Text(lithotype[n - start].value().get<std::string>().c_str());
                    ++n;
                }

                ImGui::EndTable();
            }
            ImGui::End();
        }

        if (ImGui::Begin("Capillarimetry"))
        {
            hpr::darray<double> P_c { 0.025, 0.05, 0.1, 0.3, 0.5, 0.7, 1.0};
            hpr::darray<double> data;

            if (ImGui::BeginTabBar("Graphs", ImGuiTabBarFlags_None))
            {
                const hpr::darray<std::string> vars {"K_w", "P_n"};
                std::map<std::string, hpr::darray<std::string>> cols {
                    {"K_w", hpr::darray<std::string>{"S", "V", "Y", "AB", "AE", "AH", "AK"}},
                    {"P_n", hpr::darray<std::string>{"U", "X", "AA", "AD", "AG", "AJ", "AM"}}
                };

                for (const auto& var : vars)
                if (ImGui::BeginTabItem(var.data()))
                {
                    //ImPlot::SetNextAxesToFit();
                    if (ImPlot::BeginPlot(var.data(), ImVec2(-1, -1) ))
                    {
                        ImPlot::SetupAxes("P_c",var.data());

                        for (const auto& col : cols[var])
                            data.push(wks.cell(col + std::to_string(selected)).value().get<double>());

                        ImPlot::PlotLine(var.data(), P_c.data(), data.data(), data.size());

                        ImPlot::EndPlot();
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         window.swapBuffers();
         window.pollEvents();
    }

    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}