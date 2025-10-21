#pragma once

#include "ClaudeEngine/Renderer/Framebuffer.h"
#include "ClaudeEngine/Renderer/EditorCamera.h"
#include "ClaudeEngine/Renderer/Shader.h"
#include "ClaudeEngine/Renderer/VertexArray.h"
#include "ClaudeEngine/Scene/Scene.h"
#include "ClaudeEngine/Scene/Entity.h"
#include <glm/glm.hpp>
#include <memory>

namespace ClaudeEngine {

    enum class GizmoOperation {
        None = 0,
        Translate,
        Rotate,
        Scale
    };

    class ViewportPanel {
    public:
        ViewportPanel();
        ~ViewportPanel() = default;

        void OnUpdate(float deltaTime);
        void OnImGuiRender();
        void OnEvent(class Event& e);
        void SetContext(std::shared_ptr<Scene> scene) { m_Scene = scene; }
        void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

        std::shared_ptr<Framebuffer> GetFramebuffer() const { return m_Framebuffer; }
        void SetFramebuffer(std::shared_ptr<Framebuffer> framebuffer) { m_Framebuffer = framebuffer; }
        glm::vec2 GetViewportSize() const { return m_ViewportSize; }
        bool IsViewportFocused() const { return m_ViewportFocused; }
        bool IsViewportHovered() const { return m_ViewportHovered; }

        void SetGizmoOperation(GizmoOperation operation) { m_GizmoOperation = operation; }
        GizmoOperation GetGizmoOperation() const { return m_GizmoOperation; }

        EditorCamera& GetEditorCamera() { return m_EditorCamera; }

    private:
        void RenderGizmos();
        void RenderGrid();
        void RenderScene();

    private:
        std::shared_ptr<Framebuffer> m_Framebuffer;
        std::shared_ptr<Scene> m_Scene;
        Entity m_SelectedEntity;

        glm::vec2 m_ViewportSize = { 0, 0 };
        glm::vec2 m_ViewportBounds[2];

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

        // Editor camera
        EditorCamera m_EditorCamera;

        // Grid rendering
        std::shared_ptr<Shader> m_GridShader;
        std::shared_ptr<VertexArray> m_GridVAO;

        GizmoOperation m_GizmoOperation = GizmoOperation::Translate;
        bool m_SnapEnabled = false;
        float m_SnapValues[3] = { 0.5f, 15.0f, 0.1f }; // Translate, Rotate, Scale
    };

}
