/*

 */
#pragma once

#include <hpr/gpu.hpp>
#include "shaders.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "ui.hpp"

using namespace hpr;

Entity testEntity(gpu::ShaderProgram* shaderProgram)
{
    darray<vec3> vertices {
            /*vec2(-0.5f,  0.5f),
            vec2(0.5f,  0.5f),
            vec2(0.5f, -0.5f),
            vec2(-0.5f, -0.5f)*/
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(-0.5f,  0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),

            vec3(-0.5f, -0.5f,  0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f, -0.5f,  0.5f),

            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(-0.5f, -0.5f, -0.5f),
            vec3(-0.5f, -0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),

            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),

            vec3(-0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f, -0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(0.5f, -0.5f,  0.5f),
            vec3(-0.5f, -0.5f,  0.5f),
            vec3(-0.5f, -0.5f, -0.5f),

            vec3(-0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f, -0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f,  0.5f),
            vec3(-0.5f,  0.5f, -0.5f)
    };
    darray<vec3> normals {
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),
            vec3(0.0f,  0.0f, -1.0f),

            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),
            vec3(0.0f,  0.0f,  1.0f),

            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),
            vec3(-1.0f,  0.0f,  0.0f),

            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),
            vec3(1.0f,  0.0f,  0.0f),

            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),
            vec3(0.0f, -1.0f,  0.0f),

            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f),
            vec3(0.0f,  1.0f,  0.0f)
    };
    darray<vec3> colors {
            vec3(1.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f),
            vec3(0.0f, 0.0f, 1.0f),
            vec3(1.0f, 1.0f, 1.0f)
    };
    darray<Vector<unsigned int, 3>> indices {
            Vector<unsigned int, 3>(0, 1, 2),
            Vector<unsigned int, 3>(2, 3, 0)
    };

    Entity entity;
    entity.addVertices(vertices);
    entity.addNormals(normals);
    return entity;
}

struct WindowDockUI
{
    //virtual
    //void render() = 0;
    virtual
    void ui() = 0;
};
struct Viewport3D //: WindowDockUI
{
    gpu::ShaderProgram shaderProgram;
    gpu::ColorBuffer colorBuffer;
    gpu::DepthBuffer depthBuffer;
    gpu::Framebuffer framebuffer;
    OrbitCamera camera;
    vec4 backgroundColor;

    Viewport3D() :
        shaderProgram {},
        colorBuffer {},
        depthBuffer {},
        framebuffer {},
        camera {},
        backgroundColor {0.2f, 0.2f, 0.2f, 1.0f}
    {
        gpu::Shader vertexShader {gpu::Shader::Type::Vertex, vertexSource};
        vertexShader.create();

        gpu::Shader fragmentShader {gpu::Shader::Type::Fragment, fragmentSource};
        fragmentShader.create();

        shaderProgram.create();
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragmentShader);
        shaderProgram.link();


        vertexShader.destroy();
        fragmentShader.destroy();

        framebuffer.create();
    }
    ~Viewport3D()
    {

        //delete camera;
    }
    void destroy()
    {
        shaderProgram.destroy();
        framebuffer.destroy();
    }
    float width() const
    {
        return static_cast<float>(framebuffer.width());
    }
    void width(float w)
    {
        framebuffer.width() = static_cast<int>(w);
    }
    float height() const
    {
        return static_cast<float>(framebuffer.height());
    }
    void height(float h)
    {
        framebuffer.height() = static_cast<int>(h);
    }
    void render(Entity* entity)
    {
        framebuffer.bind();
        framebuffer.rescale();

        colorBuffer.clear(backgroundColor);
        depthBuffer.clear();

        camera.aspect() = width() / height();

        if (entity)
            entity->render(&shaderProgram);

        shaderProgram.bind();

        // camera
        shaderProgram.uniformMatrix<4, 4>("view", 1, true, camera.view().data());
        shaderProgram.uniformMatrix<4, 4>("projection", 1, true, camera.projection().data());
        shaderProgram.uniformVector<float, 3>("viewPos", 1,  camera.position().data());

        // light
        hpr::vec3 lightColor {1.0f, 1.0f, 1.0f};
        shaderProgram.uniformVector<float, 3>("lightColor", 1,  lightColor.data());
        hpr::vec3 lightPos {1.0f, 1.0f, 1.0f};
        shaderProgram.uniformVector<float, 3>("lightPos", 1,  lightPos.data());

        shaderProgram.unbind();

        framebuffer.unbind();
    }
    void ui() //override
    {
        if (!ImGui::IsWindowCollapsed())
        {
            width(ImGui::GetContentRegionAvail().x);
            height(ImGui::GetContentRegionAvail().y);

            ImGui::Image(reinterpret_cast<void *>(framebuffer.texture().index()),
                         ImGui::GetContentRegionAvail(),ImVec2{0, 1}, ImVec2{1, 0});

            ImGuiIO& io = ImGui::GetIO();

            if (ImGui::IsWindowHovered())
            {
                if (io.WantCaptureMouse)
                {
                    camera.scrollEvent(0, io.MouseWheel);

                    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                    {
                        if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                        {
                            camera.moveEvent(io.MouseDelta.x, io.MouseDelta.y);
                        }
                        else
                        {
                            camera.rotateEvent(io.MouseDelta.x, io.MouseDelta.y);
                        }
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {}
                }
            }
        }
    }
};


/*struct Viewport3DUI : public WindowDockUI
{
    Viewport3D* viewport3D;
    explicit
    Viewport3DUI(Viewport3D* viewport3D) :
        viewport3D {viewport3D}
    {}
    void render() override
    {
        if (!ImGui::IsWindowCollapsed())
        {
            viewport3D->width(ImGui::GetContentRegionAvail().x);
            viewport3D->height(ImGui::GetContentRegionAvail().y);

            ImGui::Image(reinterpret_cast<void *>(viewport3D->framebuffer.texture().index()),
                         ImGui::GetContentRegionAvail(),ImVec2{0, 1}, ImVec2{1, 0});

            ImGuiIO& io = ImGui::GetIO();

            if (ImGui::IsWindowHovered())
            {
                if (io.WantCaptureMouse)
                {
                    OrbitCamera* orbitCamera = dynamic_cast<OrbitCamera*>(viewport3D->camera);
                    orbitCamera->scrollEvent(0, io.MouseWheel);

                    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                    {
                        if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                        {
                            orbitCamera->moveEvent(io.MouseDelta.x, io.MouseDelta.y);
                        }
                        else
                        {
                            orbitCamera->rotateEvent(io.MouseDelta.x, io.MouseDelta.y);
                        }
                    }

                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {}
                }
            }
        }
    }
};*/

struct Outliner //: WindowDockUI
{
    darray<Entity*> entities;

    Outliner() :
        entities {}
    {}

    ~Outliner()
    {
        for (auto entity : entities)
            delete entity;
    }

    void ui()
    {
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;

        static int selection_mask = (1 << 2);
        int node_clicked = -1;

        for (int n = 0; n < entities.size(); ++n)
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            const bool is_selected = (selection_mask & (1 << n)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;

            auto label = entities[n]->label() == "" ? std::string("Entity") : entities[n]->label();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 5.f));
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)n, node_flags, "%s %s", ICON_FA_CUBE, label.data());
            ImGui::PopStyleVar();

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = n;

            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", nullptr, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
            if (node_open)
            {
                ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
                ImGui::Text(label.data());
                ImGui::TreePop();
            }

        }
        if (node_clicked != -1)
        {
            if (ImGui::GetIO().KeyShift)
                selection_mask ^= (1 << node_clicked);
            else
                selection_mask = (1 << node_clicked);
        }


        //static bool selection[entities.size()] ;//= { false, false, false, false, false };
        /*for (int n = 0; n < entities.size(); ++n)
        {
            char buf[32];
            sprintf(buf, "Object %d", n);
            if (ImGui::Selectable(buf, entities[n]->selected))
            {
                if (!ImGui::GetIO().KeyShift)
                    for (auto& entity : entities)
                        entity->selected = false;
                entities[n]->selected ^= 1;
            }
            auto label = entities[n]->label() == "" ? std::string("Entity") : entities[n]->label();
            ImGui::SameLine();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.f, 5.f));
            if (ImGui::TreeNodeEx((std::string(ICON_FA_CUBE " ") + label + "##" + std::to_string(n)).c_str(), ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth))
            {
                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
            //ImGui::SameLine();
            //ImGui::Button("asd");
        }*/
        /*static int selected_object = -1;
        if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_BordersOuterV)) //  | ImGuiTableFlags_Resizable
        {
            ImGui::TableSetupColumn("Object", ImGuiTableColumnFlags_NoHide);
            //ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 20.0f);
            //ImGui::TableHeadersRow();

            for (int n = 0; n < entities.size(); n++) {
                ImGui::TableNextRow();
                //ImGui::TableNextColumn();

                auto label = entities[n]->label() == "" ? std::string("Entity") : entities[n]->label();

                bool tnex_open = ImGui::TreeNodeEx((label + "##" + std::to_string(n)).c_str(),
                                                   ImGuiTreeNodeFlags_SpanFullWidth);

                if (ImGui::IsItemClicked()) {
                    selected_object = n;
                }
                if (ImGui::BeginPopupContextItem()) {
                    selected_object = n;

                    if (ImGui::Selectable("Remove")) {
                        //scene.remove_object(n);
                        selected_object = -1;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                //ImGui::TableNextColumn();
                ImGui::SameLine();
                ImGui::TextDisabled("#%d", n);

                if (tnex_open) {
                    ImGui::TableNextRow();
                    //ImGui::TableNextColumn();
                    ImGui::TreeNodeEx("asdasd",//std::to_string(scene.object(n).get_id()).c_str(),
                                      ImGuiTreeNodeFlags_Leaf | //ImGuiTreeNodeFlags_Bullet |
                                      ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                    //ImGui::TableNextColumn();

                    ImGui::TreePop();
                }

            }

            ImGui::EndTable();
        }*/
    }
};
struct WindowDock
{
    enum class Type
    {
        Viewport3d,
        Outliner,
        Properties
    };

    struct Component
    {
        unsigned int id;
        unsigned int nodeId;
        Type type;
        Viewport3D viewport3D;
        Outliner *outliner;
    };

    darray<Component*> components;

    WindowDock() :
        components {}
    {}

    ~WindowDock() = default;

    void destroy()
    {
        for (auto& component : components)
        {
            component->viewport3D.destroy();
            delete component;
            component = nullptr;
        }
    }

    unsigned int lastID()
    {
        unsigned int last = 0;
        for (auto c : components)
            if (c->id > last)
                last = c->id;
        return last;
    }

    void render()
    {
        std::string icon;

        bool splitHorizontal;
        bool splitVertical;
        bool close;

        for (auto& component : components)
        {
            switch (component->type)
            {
                case Type::Viewport3d: icon = ICON_FA_CHART_AREA ICON_FA_ANGLE_DOWN; break;
                case Type::Outliner: icon = ICON_FA_LIST ICON_FA_ANGLE_DOWN; break;
                case Type::Properties: icon = ICON_FA_PAPERCLIP ICON_FA_ANGLE_DOWN; break;
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            if (ImGui::Begin((std::string("Component#") + std::to_string(component->id)).data(), nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar))
            {
                ImGui::PopStyleVar();

                bool hovered = ImGui::IsWindowHovered();

                if (ImGui::BeginMenuBar())
                {
                    //if (hovered)
                    //    ImGui::PopStyleColor();

                    if (ImGui::BeginMenu(icon.data()))
                    {
                        ImGui::TextDisabled("General");
                        ImGui::Separator();
                        if (ImGui::MenuItem(ICON_FA_CHART_AREA " 3D Viewport"))
                            component->type = Type::Viewport3d;

                        ImGui::TextDisabled("Data");
                        ImGui::Separator();
                        if (ImGui::MenuItem(ICON_FA_LIST " Outliner"))
                            component->type = Type::Outliner;
                        if (ImGui::MenuItem(ICON_FA_PAPERCLIP " Properties"))
                            component->type = Type::Properties;

                        ImGui::EndMenu();
                    }


                    ImGuiStyle& style = ImGui::GetStyle();
                    float rightAlignedWidth = ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(ICON_FA_GEAR).x + style.FramePadding.x * 2.f + style.ItemSpacing.x);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rightAlignedWidth);
                    if (ImGui::BeginMenu(ICON_FA_GEAR))
                    {
                        splitHorizontal = ImGui::MenuItem("Split horizontal");
                        splitVertical = ImGui::MenuItem("Split vertical");
                        close = ImGui::MenuItem("Close");
                        ImGui::EndMenu();
                    }

                    ImGui::EndMenuBar();
                }

                switch (component->type)
                {
                    case Type::Viewport3d:
                        //if (viewport3D != nullptr)
                            component->viewport3D.ui();
                        break;
                    case Type::Outliner: component->outliner->ui(); break;
                    case Type::Properties: break;
                }


                ImGui::End();
            }

            auto split = [](ImGuiID& parentNode, ImGuiDir direction, unsigned int currentID, unsigned int lastID)
            {
                //ImGui::DockBuilderSetNodePos(parent_node, ImGui::GetWindowPos());
                //ImGui::DockBuilderSetNodeSize(parent_node, ImGui::GetWindowSize());
                ImGuiID nodeA;
                ImGuiID nodeB;
                ImGui::DockBuilderSplitNode(parentNode, direction, 0.5f, &nodeB, &nodeA);

                ImGui::DockBuilderDockWindow((std::string("Component#") + std::to_string(currentID)).data(), nodeA);
                ImGui::DockBuilderDockWindow((std::string("Component#") + std::to_string(lastID + 1)).data(), nodeB);

                ImGuiDockNode* node = ImGui::DockBuilderGetNode(nodeB);
                node->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoTabBar;

                ImGui::DockBuilderFinish(parentNode);
                parentNode = nodeA;

                return nodeB;
            };

            if (splitHorizontal)
            {
                auto newNode = split(component->nodeId, ImGuiDir_Right, component->id, lastID());
                components.push(new Component{lastID() + 1, newNode, component->type, {}, component->outliner});
                splitHorizontal = false;
            }

            if (splitVertical)
            {
                auto newNode = split(component->nodeId, ImGuiDir_Down, component->id, lastID());
                components.push(new Component{lastID() + 1, newNode, component->type, {}, component->outliner});
                splitVertical = false;
            }

            if (close)
            {
                component = nullptr;
                close = false;
            }
        }

        components.remove([](Component* component)
        {
            return component == nullptr;
        });
    }
};

/*struct OutlinerUI : public WindowDockUI
{
    Outliner* outliner;
    explicit
    OutlinerUI(Outliner* outliner) :
        outliner {outliner}
    {}
    void render()
    {

    }
};*/
struct Properties
{
    Properties()
    {}

    ~Properties()
    {}

    void ui()
    {
        
    }
};

struct Application
{

    gpu::Window window;
    gpu::Viewport viewport;
    gpu::ColorBuffer colorBuffer;
    gpu::DepthBuffer depthBuffer;
    //Viewport3D viewport3D;
    Outliner outliner;
    Properties properties;
    vec4 backgroundColor;
    UI ui;

    Application() :
        window {100, 100, 1200, 900, "Hyporo", gpu::Window::Windowed, nullptr, nullptr},
        viewport {},
        colorBuffer {},
        depthBuffer {},
        //viewport3D {},
        outliner {},
        properties {},
        backgroundColor {0.2f, 0.2f, 0.2f, 1.0f},
        ui {&window}
    {
        window.framebufferResizeCallback([](gpu::Window* w, int width, int height)
        {
            w->size(width, height);
        });
    }

    ~Application()
    {
        window.destroy();
    }

    void run()
    {
        window.context().debug();
        window.icon("icon.png");

        depthBuffer.bind();
        glEnable(GL_PROGRAM_POINT_SIZE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        /*darray<WindowDock> windowDocks {
            {1, WindowDock::Type::Viewport3d,  &outliner},
            {2, WindowDock::Type::Outliner, &outliner},
            {3, WindowDock::Type::Properties, &outliner}
        };*/
        WindowDock windowDock;
        windowDock.components.push(new WindowDock::Component{1, 1, WindowDock::Type::Viewport3d, {}, &outliner});
        windowDock.components.push(new WindowDock::Component{2, 3, WindowDock::Type::Outliner, {}, &outliner});
        windowDock.components.push(new WindowDock::Component{3, 4, WindowDock::Type::Properties, {}, &outliner});

        auto sp = Viewport3D().shaderProgram;
        //auto ent = testEntity(&sp);
        outliner.entities.push(new Entity(testEntity(&sp)));


        while (window.state() != gpu::Window::Closed)
        {
            if (window.shouldClose())
                window.state(gpu::Window::Closed);

            // Clean up buffers
            colorBuffer.clear(backgroundColor);
            depthBuffer.clear();

            for (auto& component : windowDock.components)
            {
                // Viewport3D
                if (component->type != WindowDock::Type::Viewport3d)
                    continue;
                viewport.size() = vec2(component->viewport3D.width(), component->viewport3D.height());
                viewport.set();
                component->viewport3D.render(nullptr);
                for (auto entity: outliner.entities)
                    component->viewport3D.render(entity);
            }
            // Main window
            viewport.size() = vec2(window.width(), window.height());
            viewport.set();

            // UI
            ui.createFrame();

            ui.mainMenu();
            ui.mainDockspace();
            windowDock.render();
            /*for (auto& windowDock : windowDocks)
            {
                switch (windowDock.type)
                {
                    case (WindowDock::Type::Viewport3d):
                    {
                        windowDock.render();
                        break;
                    }
                    case (WindowDock::Type::Outliner):
                    {
                        //auto outlinerUI = OutlinerUI(&outliner);
                        windowDock.render();//&outliner);
                        break;
                    }
                    case (WindowDock::Type::Properties): break;
                }
            }*/
            /*windowDocks.remove([](WindowDock wd){
                return wd.close;
            });*/
            /*unsigned int lastID = 0;
            for (auto wd : windowDocks)
                if (wd.id > lastID)
                    lastID = wd.id;
            for (auto n = 0; n < windowDocks.size(); ++n)
            {
                if (windowDocks[n].splitHorizontal)
                {
                    if (ImGui::Begin("#Dockspace"))
                    {
                        auto nodeID = ImGui::GetID((std::string("WindowDock#") + std::to_string(windowDocks[n].id)).data());
                        //auto* node = ImGui::DockBuilderGetNode(nodeID);
                        if (ImGui::DockBuilderGetNode(nodeID) == nullptr)
                        {
                            //ImGui::DockBuilderRemoveNode(nodeID);
                            ImGui::DockBuilderAddNode(nodeID, ImGuiDockNodeFlags_DockSpace);
                            ImGui::DockBuilderSetNodeSize(nodeID, ImVec2(350.f, 350.f));
                            ImGuiID newDock = ImGui::DockBuilderSplitNode(nodeID, ImGuiDir_Right, 0.2f, nullptr,
                                                                          &nodeID);
                            //ImGui::DockBuilderDockWindow((std::string("WindowDock#") + std::to_string(nodeID)).data(),
                            //                             nodeID);
                            ImGui::DockBuilderDockWindow(
                                    (std::string("WindowDock#") + std::to_string(lastID + 1)).data(), newDock);
                            windowDocks.push({lastID + 1, windowDocks[n].type, &outliner});
                        }
                        windowDocks[n].splitHorizontal = false;
                        ImGui::End();
                    }
                }
            }*/
            ui.statusBar();

            ui.renderFrame();
            //
            window.swapBuffers();
            window.pollEvents();
        }

        windowDock.destroy();
    }
};
