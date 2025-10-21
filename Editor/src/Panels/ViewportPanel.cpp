#include "Panels/ViewportPanel.h"
#include "ClaudeEngine/Scene/Components.h"
#include "ClaudeEngine/Renderer/RenderCommand.h"
#include "ClaudeEngine/Renderer/Renderer3D.h"
#include "ClaudeEngine/Renderer/Model.h"
#include "ClaudeEngine/Core/Log.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ClaudeEngine {

    ViewportPanel::ViewportPanel() 
        : m_EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f) {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
        m_Framebuffer = Framebuffer::Create(fbSpec);
    }

    void ViewportPanel::OnUpdate(float deltaTime) {
        // Update viewport size
        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

        // Update editor camera with ImGui input (but not when gizmo is being used)
        if ((m_ViewportFocused || m_ViewportHovered) && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver()) {
            auto& io = ImGui::GetIO();
            
            bool rightMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);
            bool middleMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Middle);
            
            bool wKey = ImGui::IsKeyDown(ImGuiKey_W);
            bool sKey = ImGui::IsKeyDown(ImGuiKey_S);
            bool aKey = ImGui::IsKeyDown(ImGuiKey_A);
            bool dKey = ImGui::IsKeyDown(ImGuiKey_D);
            bool qKey = ImGui::IsKeyDown(ImGuiKey_Q);
            bool eKey = ImGui::IsKeyDown(ImGuiKey_E);
            
            m_EditorCamera.OnUpdate(deltaTime, rightMouseDown, middleMouseDown,
                                   io.MousePos.x, io.MousePos.y, io.MouseWheel,
                                   wKey, sKey, aKey, dKey, qKey, eKey);
        }

        // Render scene to framebuffer
        if (m_ViewportSize.x > 0 && m_ViewportSize.y > 0) {
            RenderScene();
        }
    }

    void ViewportPanel::OnEvent(Event& e) {
        // Events are handled through ImGui input in OnUpdate
    }

    void ViewportPanel::RenderScene() {
        // Bind framebuffer
        m_Framebuffer->Bind();

        // Clear
        RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.4f, 1.0f });
        RenderCommand::Clear();

        // Begin 3D rendering
        Renderer3D::BeginScene(m_EditorCamera);

        // Draw grid
        Renderer3D::DrawGrid();

        // Render all entities in the scene
        if (m_Scene) {
            auto& registry = m_Scene->GetRegistry();
            auto view = registry.view<TransformComponent>();
            for (auto entityHandle : view) {
                Entity entity{ entityHandle, m_Scene.get() };
                
                auto& transform = entity.GetComponent<TransformComponent>();
                
                // Render MeshRenderer components
                if (entity.HasComponent<MeshRendererComponent>()) {
                    auto& mr = entity.GetComponent<MeshRendererComponent>();
                    if (mr.ModelAsset && mr.Visible) {
                        Renderer3D::DrawModel(mr.ModelAsset, transform.GetTransform());
                    }
                }
                
                // Draw primitive cubes for entities without models (for debugging)
                else {
                    Renderer3D::DrawCube(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
                }
            }
        }

        // End 3D rendering
        Renderer3D::EndScene();

        // Unbind framebuffer
        m_Framebuffer->Unbind();
    }

    void ViewportPanel::OnImGuiRender() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        glm::vec2 viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        if (m_ViewportSize != viewportSize && viewportSize.x > 0 && viewportSize.y > 0) {
            m_ViewportSize = viewportSize;
            m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        }

        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        
        // Get the position of the image (viewport content area)
        ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        ImVec2 viewportOffset = ImGui::GetWindowPos();
        
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
        
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

        // Render gizmos
        RenderGizmos();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ViewportPanel::RenderGizmos() {
        // Skip if no entity is selected
        if (!m_SelectedEntity || m_GizmoOperation == GizmoOperation::None)
            return;

        // Check if entity has transform component
        if (!m_SelectedEntity.HasComponent<TransformComponent>())
            return;

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        // Set ImGuizmo rect to match viewport
        ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y,
                         m_ViewportBounds[1].x - m_ViewportBounds[0].x,
                         m_ViewportBounds[1].y - m_ViewportBounds[0].y);

        // Get camera matrices
        const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
        glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

        // Get entity transform
        auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
        glm::mat4 transform = tc.GetTransform();

        // Snap values
        bool snap = m_SnapEnabled;
        float snapValue = 0.5f; // Default for translate
        if (m_GizmoOperation == GizmoOperation::Rotate)
            snapValue = m_SnapValues[1];
        else if (m_GizmoOperation == GizmoOperation::Scale)
            snapValue = m_SnapValues[2];
        else
            snapValue = m_SnapValues[0];

        float snapValues[3] = { snapValue, snapValue, snapValue };

        // Convert operation
        ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
        if (m_GizmoOperation == GizmoOperation::Translate)
            operation = ImGuizmo::TRANSLATE;
        else if (m_GizmoOperation == GizmoOperation::Rotate)
            operation = ImGuizmo::ROTATE;
        else if (m_GizmoOperation == GizmoOperation::Scale)
            operation = ImGuizmo::SCALE;

        // Manipulate gizmo
        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                            operation, ImGuizmo::LOCAL, glm::value_ptr(transform),
                            nullptr, snap ? snapValues : nullptr);

        // Update entity transform if gizmo is being used
        if (ImGuizmo::IsUsing()) {
            glm::vec3 translation, rotation, scale;
            // Decompose matrix
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat orientation;
            glm::decompose(transform, scale, orientation, translation, skew, perspective);
            rotation = glm::eulerAngles(orientation);

            tc.Translation = translation;
            tc.Rotation = rotation;
            tc.Scale = scale;
        }
    }

}
