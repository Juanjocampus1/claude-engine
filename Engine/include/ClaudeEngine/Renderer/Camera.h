#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace ClaudeEngine {

    class Camera {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_Projection(projection) {}

        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_Projection; }
        void SetProjection(const glm::mat4& projection) { m_Projection = projection; }

    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };

    class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);

        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetRotation() const { return m_Rotation; }

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjection() const { return m_ViewProjection; }

        glm::vec3 GetForward() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

    private:
        void RecalculateViewMatrix();

    private:
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjection;
    };

}
