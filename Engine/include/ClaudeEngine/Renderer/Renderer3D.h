#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "ClaudeEngine/Renderer/Shader.h"
#include "ClaudeEngine/Renderer/VertexArray.h"
#include "ClaudeEngine/Renderer/EditorCamera.h"
#include "ClaudeEngine/Scene/Scene.h"
#include <glm/glm.hpp>

namespace ClaudeEngine {

    // Forward declarations
    class Model;

    class Renderer3D {
    public:
        static void Init();
        static void Shutdown();

        // Begin/End scene with editor camera
        static void BeginScene(const EditorCamera& camera);
        static void EndScene();

        // Primitives
        static void DrawGrid();
        static void DrawCube(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawModel(const Ref<Model>& model, const glm::mat4& transform);

        // Gizmos
        static void DrawGizmo(const glm::mat4& transform, int gizmoOperation, int gizmoMode);

        // Stats
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t Vertices = 0;
            uint32_t Triangles = 0;
        };
        static Statistics GetStats();
        static void ResetStats();

    private:
        static void InitGrid();
        static void InitCube();
    };

}
