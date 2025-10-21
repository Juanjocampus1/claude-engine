#include "ClaudeEngine/Renderer/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ClaudeEngine {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) {
        SetPerspective(fov, aspectRatio, nearClip, farClip);
    }

    void PerspectiveCamera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip) {
        m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        m_ViewProjection = m_Projection * m_ViewMatrix;
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetRotation(const glm::vec3& rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    glm::vec3 PerspectiveCamera::GetForward() const {
        return glm::rotate(glm::quat(m_Rotation), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 PerspectiveCamera::GetRight() const {
        return glm::rotate(glm::quat(m_Rotation), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 PerspectiveCamera::GetUp() const {
        return glm::rotate(glm::quat(m_Rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void PerspectiveCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
        transform *= glm::toMat4(glm::quat(m_Rotation));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjection = m_Projection * m_ViewMatrix;
    }

}
