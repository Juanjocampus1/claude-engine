#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "ClaudeEngine/Scene/Scene.h"
#include "ClaudeEngine/Scene/Entity.h"
#include "ClaudeEngine/Renderer/Framebuffer.h"
#include <imgui.h>
#include <functional>
#include <filesystem>
#include <glm/glm.hpp>

namespace ClaudeEngine {

    // ========== HIERARCHY PANEL ==========
    class HierarchyPanel {
    public:
        HierarchyPanel() = default;
        HierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);
        void OnImGuiRender();

        Entity GetSelectedEntity() const { return m_SelectionContext; }
        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };

    // ========== CONTENT BROWSER PANEL (Unity-style) ==========
    class ContentBrowserPanel {
    public:
        ContentBrowserPanel();

        void OnImGuiRender();

        std::string GetCurrentDirectory() const { return m_CurrentDirectory.string(); }
        void SetRootDirectory(const std::filesystem::path& path);

    private:
        void DrawDirectoryTree(const std::filesystem::path& path);
        void DrawContents();

    private:
        std::filesystem::path m_RootDirectory;
        std::filesystem::path m_CurrentDirectory;
        
        // Icons (to be loaded in the future)
        // Ref<Texture2D> m_FolderIcon;
        // Ref<Texture2D> m_FileIcon;
        // Ref<Texture2D> m_ModelIcon;
        // Ref<Texture2D> m_TextureIcon;
    };

    // ========== PROPERTIES PANEL ==========
    class PropertiesPanel {
    public:
        PropertiesPanel() = default;

        void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }
        void OnImGuiRender();

    private:
        template<typename T, typename UIFunction>
        void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

        void DrawTransformComponent(Entity entity);
        void DrawMeshRendererComponent(Entity entity);
        void DrawLightComponent(Entity entity);
        void DrawCameraComponent(Entity entity);

    private:
        Entity m_SelectedEntity;
    };

    // ========== SETTINGS PANEL (Blender-style) ==========
    class SettingsPanel {
    public:
        SettingsPanel();

        void OnImGuiRender();

    private:
        void DrawRenderSettings();
        void DrawPhysicsSettings();
        void DrawEditorSettings();
        void DrawProjectSettings();

    private:
        // Render settings
        bool m_VSync = true;
        int m_MSAASamples = 4;
        float m_ResolutionScale = 1.0f;
        bool m_UseHDR = true;
        float m_Exposure = 1.0f;
        
        // Physics settings
        glm::vec3 m_Gravity = { 0.0f, -9.81f, 0.0f };
        float m_FixedTimestep = 0.02f;
        int m_MaxSubsteps = 8;
        
        // Editor settings
        float m_CameraSpeed = 5.0f;
        float m_CameraSensitivity = 0.1f;
        bool m_ShowGrid = true;
        bool m_ShowGizmos = true;
        
        // Active tab
        int m_ActiveTab = 0;
    };

    // ========== STATS PANEL ==========
    class StatsPanel {
    public:
        StatsPanel() = default;

        void OnImGuiRender();

    private:
        // Performance metrics
        float m_FrameTime = 0.0f;
        int m_DrawCalls = 0;
        int m_Vertices = 0;
        int m_Triangles = 0;
    };

}
