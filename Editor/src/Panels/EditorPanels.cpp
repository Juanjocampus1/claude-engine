#include "EditorPanels.h"
#include "ClaudeEngine/Scene/Components.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

namespace ClaudeEngine {

    // ========== HIERARCHY PANEL ==========

    HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene) {
        SetContext(scene);
    }

    void HierarchyPanel::SetContext(const Ref<Scene>& scene) {
        m_Context = scene;
        m_SelectionContext = {};
    }

    void HierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Hierarchy");

        if (m_Context) {
            // Create entity button
            if (ImGui::Button("Create Entity")) {
                m_Context->CreateEntity("Empty Entity");
            }

            ImGui::SameLine();
            if (ImGui::Button("Create Camera")) {
                auto entity = m_Context->CreateEntity("Camera");
                entity.AddComponent<CameraComponent>();
            }

            ImGui::SameLine();
            if (ImGui::Button("Create Light")) {
                auto entity = m_Context->CreateEntity("Light");
                entity.AddComponent<LightComponent>();
            }

            ImGui::Separator();
            
            // Search bar
            static char searchBuffer[256] = "";
            ImGui::SetNextItemWidth(-1);
            ImGui::InputTextWithHint("##Search", "Search entities...", searchBuffer, sizeof(searchBuffer));
            std::string searchStr(searchBuffer);
            
            ImGui::Separator();

            // Draw all entities
            auto view = m_Context->GetRegistry().view<TagComponent>();
            for (auto entityID : view) {
                Entity entity{ entityID, m_Context.get() };
                
                // Filter by search
                if (!searchStr.empty()) {
                    auto& tag = entity.GetComponent<TagComponent>().Tag;
                    if (tag.find(searchStr) == std::string::npos)
                        continue;
                }
                
                DrawEntityNode(entity);
            }

            // Right-click on blank space
            if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
                if (ImGui::MenuItem("Create Empty Entity"))
                    m_Context->CreateEntity("Empty Entity");
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

    void HierarchyPanel::DrawEntityNode(Entity entity) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

        if (ImGui::IsItemClicked()) {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;
            ImGui::EndPopup();
        }

        if (opened) {
            ImGui::TreePop();
        }

        if (entityDeleted) {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
    }

    // ========== CONTENT BROWSER PANEL ==========

    ContentBrowserPanel::ContentBrowserPanel() {
        m_RootDirectory = "assets";
        m_CurrentDirectory = m_RootDirectory;
        
        // Create assets directory if it doesn't exist
        try {
            if (!std::filesystem::exists(m_RootDirectory)) {
                std::filesystem::create_directories(m_RootDirectory);
            }
        } catch (...) {
            m_RootDirectory = ".";
            m_CurrentDirectory = m_RootDirectory;
        }

        // TODO: Load icons
    }

    void ContentBrowserPanel::SetRootDirectory(const std::filesystem::path& path) {
        if (std::filesystem::exists(path)) {
            m_RootDirectory = path;
            m_CurrentDirectory = m_RootDirectory;
        }
    }

    void ContentBrowserPanel::OnImGuiRender() {
        ImGui::Begin("Content Browser");
        
        // Validate current directory exists
        if (!std::filesystem::exists(m_CurrentDirectory)) {
            m_CurrentDirectory = m_RootDirectory;
        }

        // Navigation bar
        if (m_CurrentDirectory != m_RootDirectory) {
            if (ImGui::Button("<-")) {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
            ImGui::SameLine();
        }

        ImGui::Text("%s", m_CurrentDirectory.string().c_str());
        ImGui::Separator();

        // Split view: Directory tree (left) | Contents (right)
        ImGui::BeginChild("DirectoryTree", ImVec2(200, 0), true);
        DrawDirectoryTree(m_RootDirectory);
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("Contents");
        DrawContents();
        ImGui::EndChild();

        ImGui::End();
    }

    void ContentBrowserPanel::DrawDirectoryTree(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path))
            return;

        for (auto& directoryEntry : std::filesystem::directory_iterator(path)) {
            const auto& entryPath = directoryEntry.path();
            if (directoryEntry.is_directory()) {
                auto relativePath = std::filesystem::relative(entryPath, m_RootDirectory);
                bool selected = (entryPath == m_CurrentDirectory);
                
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
                if (selected)
                    flags |= ImGuiTreeNodeFlags_Selected;

                bool opened = ImGui::TreeNodeEx(entryPath.filename().string().c_str(), flags);
                
                if (ImGui::IsItemClicked()) {
                    m_CurrentDirectory = entryPath;
                }

                if (opened) {
                    DrawDirectoryTree(entryPath);
                    ImGui::TreePop();
                }
            }
        }
    }

    void ContentBrowserPanel::DrawContents() {
        if (!std::filesystem::exists(m_CurrentDirectory))
            return;

        float padding = 16.0f;
        float thumbnailSize = 128.0f;
        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            const auto& path = directoryEntry.path();
            std::string filenameString = path.filename().string();

            ImGui::PushID(filenameString.c_str());
            
            // Draw icon/thumbnail
            ImVec2 buttonSize(thumbnailSize, thumbnailSize);
            if (ImGui::Button(filenameString.c_str(), buttonSize)) {
                if (directoryEntry.is_directory()) {
                    m_CurrentDirectory /= path.filename();
                }
            }

            // Drag-drop source
            if (ImGui::BeginDragDropSource()) {
                std::string pathStr = path.string();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr.c_str(), pathStr.size() + 1);
                ImGui::Text("%s", filenameString.c_str());
                ImGui::EndDragDropSource();
            }

            ImGui::TextWrapped("%s", filenameString.c_str());
            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);
    }

    // ========== PROPERTIES PANEL ==========

    void PropertiesPanel::OnImGuiRender() {
        ImGui::Begin("Properties");

        if (m_SelectedEntity) {
            // Draw Tag Component (always present)
            if (m_SelectedEntity.HasComponent<TagComponent>()) {
                auto& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer) - 1);
                if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                    tag = std::string(buffer);
                }
            }
            
            ImGui::Separator();
            
            DrawTransformComponent(m_SelectedEntity);
            
            if (m_SelectedEntity.HasComponent<MeshRendererComponent>())
                DrawMeshRendererComponent(m_SelectedEntity);
            
            if (m_SelectedEntity.HasComponent<LightComponent>())
                DrawLightComponent(m_SelectedEntity);
            
            if (m_SelectedEntity.HasComponent<CameraComponent>())
                DrawCameraComponent(m_SelectedEntity);

            // Add component button
            ImGui::Separator();
            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent")) {
                if (!m_SelectedEntity.HasComponent<MeshRendererComponent>()) {
                    if (ImGui::MenuItem("Mesh Renderer")) {
                        m_SelectedEntity.AddComponent<MeshRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!m_SelectedEntity.HasComponent<LightComponent>()) {
                    if (ImGui::MenuItem("Light")) {
                        m_SelectedEntity.AddComponent<LightComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                }
                if (!m_SelectedEntity.HasComponent<CameraComponent>()) {
                    if (ImGui::MenuItem("Camera")) {
                        m_SelectedEntity.AddComponent<CameraComponent>();
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }

    void PropertiesPanel::DrawTransformComponent(Entity entity) {
        if (entity.HasComponent<TransformComponent>()) {
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& tc = entity.GetComponent<TransformComponent>();
                ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
                glm::vec3 rotation = glm::degrees(tc.Rotation);
                if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f))
                    tc.Rotation = glm::radians(rotation);
                ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f);
            }
        }
    }

    void PropertiesPanel::DrawMeshRendererComponent(Entity entity) {
        if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& mrc = entity.GetComponent<MeshRendererComponent>();
            
            ImGui::Checkbox("Cast Shadows", &mrc.CastShadows);
            ImGui::Checkbox("Receive Shadows", &mrc.ReceiveShadows);
            
            // Model path drag-drop
            ImGui::Text("Model: %s", mrc.ModelAsset ? "Loaded" : "None");
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    std::string path = (const char*)payload->Data;
                    // TODO: Load model
                }
                ImGui::EndDragDropTarget();
            }
        }
    }

    void PropertiesPanel::DrawLightComponent(Entity entity) {
        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& lc = entity.GetComponent<LightComponent>();
            
            const char* types[] = { "Directional", "Point", "Spot", "Area" };
            int currentType = (int)lc.Type;
            if (ImGui::Combo("Type", &currentType, types, 4))
                lc.Type = (LightType)currentType;
            
            ImGui::ColorEdit3("Color", glm::value_ptr(lc.Color));
            ImGui::DragFloat("Intensity", &lc.Intensity, 0.1f, 0.0f, 100.0f);
            
            if (lc.Type == LightType::Point || lc.Type == LightType::Spot) {
                ImGui::DragFloat("Range", &lc.Range, 0.1f, 0.0f, 100.0f);
            }
            
            if (lc.Type == LightType::Spot) {
                ImGui::DragFloat("Inner Angle", &lc.InnerConeAngle, 0.1f, 0.0f, 90.0f);
                ImGui::DragFloat("Outer Angle", &lc.OuterConeAngle, 0.1f, 0.0f, 90.0f);
            }
            
            if (lc.Type == LightType::Area) {
                ImGui::DragFloat2("Size", glm::value_ptr(lc.AreaSize), 0.1f, 0.1f, 10.0f);
            }
        }
    }

    void PropertiesPanel::DrawCameraComponent(Entity entity) {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& cc = entity.GetComponent<CameraComponent>();
            
            ImGui::Checkbox("Primary", &cc.Primary);
            ImGui::DragFloat("FOV", &cc.FOV, 0.1f, 1.0f, 179.0f);
            ImGui::DragFloat("Near Clip", &cc.NearClip, 0.01f, 0.01f, 10.0f);
            ImGui::DragFloat("Far Clip", &cc.FarClip, 1.0f, 10.0f, 10000.0f);
        }
    }

    // ========== SETTINGS PANEL ==========

    SettingsPanel::SettingsPanel() {
    }

    void SettingsPanel::OnImGuiRender() {
        ImGui::Begin("Settings");

        // Tab bar (Blender-style)
        if (ImGui::BeginTabBar("SettingsTabs")) {
            if (ImGui::BeginTabItem("Render")) {
                DrawRenderSettings();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Physics")) {
                DrawPhysicsSettings();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Editor")) {
                DrawEditorSettings();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Project")) {
                DrawProjectSettings();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void SettingsPanel::DrawRenderSettings() {
        ImGui::SeparatorText("Display");
        ImGui::Checkbox("VSync", &m_VSync);
        const char* msaaLevels[] = { "Off", "2x", "4x", "8x", "16x" };
        ImGui::Combo("MSAA", &m_MSAASamples, msaaLevels, 5);
        ImGui::DragFloat("Resolution Scale", &m_ResolutionScale, 0.05f, 0.25f, 2.0f);

        ImGui::SeparatorText("Post Processing");
        ImGui::Checkbox("HDR", &m_UseHDR);
        ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.1f, 10.0f);

        ImGui::SeparatorText("Ray Tracing");
        ImGui::Text("Ray Tracing: Not implemented");
        // Future: Ray tracing settings
    }

    void SettingsPanel::DrawPhysicsSettings() {
        ImGui::SeparatorText("World");
        ImGui::DragFloat3("Gravity", glm::value_ptr(m_Gravity), 0.1f);

        ImGui::SeparatorText("Simulation");
        ImGui::DragFloat("Fixed Timestep", &m_FixedTimestep, 0.001f, 0.001f, 0.1f);
        ImGui::DragInt("Max Substeps", &m_MaxSubsteps, 1, 1, 16);
    }

    void SettingsPanel::DrawEditorSettings() {
        ImGui::SeparatorText("Camera");
        ImGui::DragFloat("Movement Speed", &m_CameraSpeed, 0.1f, 0.1f, 100.0f);
        ImGui::DragFloat("Mouse Sensitivity", &m_CameraSensitivity, 0.01f, 0.01f, 1.0f);

        ImGui::SeparatorText("Viewport");
        ImGui::Checkbox("Show Grid", &m_ShowGrid);
        ImGui::Checkbox("Show Gizmos", &m_ShowGizmos);
    }

    void SettingsPanel::DrawProjectSettings() {
        ImGui::Text("Project: Claude Engine");
        ImGui::Text("Version: 1.0.0");
        ImGui::Separator();
        
        if (ImGui::Button("Export Project")) {
            // TODO: Export functionality
        }
    }

    // ========== STATS PANEL ==========

    void StatsPanel::OnImGuiRender() {
        ImGui::Begin("Stats");

        ImGui::Text("Performance");
        ImGui::Separator();
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

        ImGui::Spacing();
        ImGui::Text("Rendering");
        ImGui::Separator();
        ImGui::Text("Draw Calls: %d", m_DrawCalls);
        ImGui::Text("Vertices: %d", m_Vertices);
        ImGui::Text("Triangles: %d", m_Triangles);

        ImGui::End();
    }

}
