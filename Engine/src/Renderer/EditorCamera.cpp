#include "ClaudeEngine/Renderer/EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace ClaudeEngine {

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip) {
        UpdateView();
        UpdateProjection();
    }

    void EditorCamera::UpdateProjection() {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::UpdateView() {
        m_Position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    std::pair<float, float> EditorCamera::PanSpeed() const {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

    void EditorCamera::OnUpdate(float deltaTime, bool rightMouseDown, bool middleMouseDown, 
                                float mouseX, float mouseY, float scrollY,
                                bool wKey, bool sKey, bool aKey, bool dKey, bool qKey, bool eKey) {
        m_CurrentMousePosition = { mouseX, mouseY };

        // WASD Movement (only when right mouse is down)
        if (rightMouseDown) {
            m_IsActive = true;
            
            glm::vec3 forward = GetForwardDirection();
            glm::vec3 right = GetRightDirection();
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

            float speed = m_MovementSpeed * deltaTime;

            if (wKey) m_FocalPoint += forward * speed;
            if (sKey) m_FocalPoint -= forward * speed;
            if (aKey) m_FocalPoint -= right * speed;
            if (dKey) m_FocalPoint += right * speed;
            if (eKey) m_FocalPoint += up * speed;
            if (qKey) m_FocalPoint -= up * speed;

            // Mouse rotation
            glm::vec2 delta = (m_CurrentMousePosition - m_InitialMousePosition);
            m_InitialMousePosition = m_CurrentMousePosition;

            if (delta.x != 0.0f || delta.y != 0.0f) {
                MouseRotate(delta);
            }
        } else {
            m_IsActive = false;
            m_InitialMousePosition = m_CurrentMousePosition;
        }

        // Middle mouse button pan
        if (middleMouseDown) {
            glm::vec2 delta = (m_CurrentMousePosition - m_InitialMousePosition);
            m_InitialMousePosition = m_CurrentMousePosition;
            MousePan(delta);
        } else {
            m_InitialMousePosition = m_CurrentMousePosition;
        }

        // Mouse scroll zoom
        if (scrollY != m_LastScrollY) {
            float delta = (scrollY - m_LastScrollY) * 0.1f;
            MouseZoom(delta);
            m_LastScrollY = scrollY;
        }

        UpdateView();
    }

    void EditorCamera::OnEvent(Event& e) {
        // Events are handled through ImGui input in OnUpdate
    }

    void EditorCamera::MousePan(const glm::vec2& delta) {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * m_MouseSensitivity;
        m_Pitch += delta.y * m_MouseSensitivity;
    }

    void EditorCamera::MouseZoom(float delta) {
        m_Distance -= delta * ZoomSpeed();
        if (m_Distance < 1.0f) {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::GetUpDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    glm::quat EditorCamera::GetOrientation() const {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

}
