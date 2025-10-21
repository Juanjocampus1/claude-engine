#include "ClaudeEngine/Renderer/Renderer3D.h"
#include "ClaudeEngine/Renderer/RenderCommand.h"
#include "ClaudeEngine/Renderer/Buffer.h"
#include "ClaudeEngine/Renderer/Model.h"
#include "ClaudeEngine/Core/Log.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace ClaudeEngine {

    struct Renderer3DData {
        // Grid
        Ref<Shader> GridShader;
        Ref<VertexArray> GridVAO;

        // Basic cube for primitives
        Ref<Shader> BasicShader;
        Ref<VertexArray> CubeVAO;

        // Scene data
        glm::mat4 ViewProjectionMatrix;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        // Stats
        Renderer3D::Statistics Stats;
    };

    static Renderer3DData* s_Data = nullptr;

    void Renderer3D::Init() {
        CE_INFO("Renderer3D::Init() called");
        s_Data = new Renderer3DData();
        CE_INFO("Renderer3D data allocated");

        InitGrid();
        InitCube();
        
        CE_INFO("Renderer3D::Init() completed successfully");
    }

    void Renderer3D::Shutdown() {
        delete s_Data;
        s_Data = nullptr;
    }

    void Renderer3D::BeginScene(const EditorCamera& camera) {
        s_Data->ViewMatrix = camera.GetViewMatrix();
        s_Data->ProjectionMatrix = camera.GetProjection();
        s_Data->ViewProjectionMatrix = camera.GetViewProjection();
        s_Data->NearClip = 0.1f; // Match camera settings
        s_Data->FarClip = 1000.0f;

        ResetStats();
    }

    void Renderer3D::EndScene() {
        // Nothing to do yet
    }

    void Renderer3D::DrawGrid() {
        if (!s_Data->GridShader || !s_Data->GridVAO) {
            CE_WARN("Renderer3D: Cannot draw grid - shader or VAO is null");
            return;
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_Data->GridShader->Bind();
        s_Data->GridShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        s_Data->GridShader->SetFloat("u_Near", s_Data->NearClip);
        s_Data->GridShader->SetFloat("u_Far", s_Data->FarClip);

        s_Data->GridVAO->Bind();
        RenderCommand::DrawIndexed(s_Data->GridVAO);

        glDisable(GL_BLEND);

        s_Data->Stats.DrawCalls++;
    }

    void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color) {
        s_Data->BasicShader->Bind();
        s_Data->BasicShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
        s_Data->BasicShader->SetMat4("u_Transform", transform);
        s_Data->BasicShader->SetFloat4("u_Color", color);

        s_Data->CubeVAO->Bind();
        RenderCommand::DrawIndexed(s_Data->CubeVAO);

        s_Data->Stats.DrawCalls++;
        s_Data->Stats.Vertices += 24;
        s_Data->Stats.Triangles += 12;
    }

    void Renderer3D::DrawModel(const Ref<Model>& model, const glm::mat4& transform) {
        if (!model) return;

        // Get meshes from model
        auto& meshes = model->GetMeshes();
        for (auto& mesh : meshes) {
            // Use basic shader for now (TODO: integrate materials)
            s_Data->BasicShader->Bind();
            s_Data->BasicShader->SetMat4("u_ViewProjection", s_Data->ViewProjectionMatrix);
            s_Data->BasicShader->SetMat4("u_Transform", transform);
            s_Data->BasicShader->SetFloat4("u_Color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

            mesh->GetVertexArray()->Bind();
            RenderCommand::DrawIndexed(mesh->GetVertexArray());

            s_Data->Stats.DrawCalls++;
            // TODO: Add vertex/triangle count from mesh
        }
    }

    void Renderer3D::DrawGizmo(const glm::mat4& transform, int gizmoOperation, int gizmoMode) {
        // Gizmos are drawn by ImGuizmo directly in ImGui context
        // This is just a placeholder
    }

    Renderer3D::Statistics Renderer3D::GetStats() {
        return s_Data->Stats;
    }

    void Renderer3D::ResetStats() {
        s_Data->Stats.DrawCalls = 0;
        s_Data->Stats.Vertices = 0;
        s_Data->Stats.Triangles = 0;
    }

    // ==================== INITIALIZATION ====================

    void Renderer3D::InitGrid() {
        CE_INFO("Renderer3D: Initializing grid...");
        
        // Load grid shader from file
        CE_INFO("Renderer3D: Loading grid shader from assets/shaders/Grid.glsl...");
        s_Data->GridShader = Shader::Create("assets/shaders/Grid.glsl");
        if (!s_Data->GridShader) {
            CE_ERROR("Renderer3D: Failed to load grid shader from file!");
            return;
        }
        CE_INFO("Renderer3D: Grid shader loaded successfully");

        // Fullscreen quad for grid
        float gridVertices[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f
        };

        uint32_t gridIndices[] = { 0, 1, 2, 2, 3, 0 };

        s_Data->GridVAO = VertexArray::Create();
        Ref<VertexBuffer> gridVB = VertexBuffer::Create(gridVertices, sizeof(gridVertices));
        gridVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_Data->GridVAO->AddVertexBuffer(gridVB);

        Ref<IndexBuffer> gridIB = IndexBuffer::Create(gridIndices, sizeof(gridIndices) / sizeof(uint32_t));
        s_Data->GridVAO->SetIndexBuffer(gridIB);
        
        CE_INFO("Renderer3D: Grid initialized successfully");
    }

    void Renderer3D::InitCube() {
        CE_INFO("Renderer3D: Initializing cube primitive...");
        
        // Load basic shader from file
        CE_INFO("Renderer3D: Loading basic color shader from assets/shaders/BasicColor.glsl...");
        s_Data->BasicShader = Shader::Create("assets/shaders/BasicColor.glsl");
        if (!s_Data->BasicShader) {
            CE_ERROR("Renderer3D: Failed to load basic color shader from file!");
            return;
        }
        CE_INFO("Renderer3D: Basic color shader loaded successfully");

        // Cube vertices (simple cube)
        float cubeVertices[] = {
            // Front
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            // Back
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f
        };

        uint32_t cubeIndices[] = {
            // Front
            0, 1, 2, 2, 3, 0,
            // Back
            5, 4, 7, 7, 6, 5,
            // Left
            4, 0, 3, 3, 7, 4,
            // Right
            1, 5, 6, 6, 2, 1,
            // Top
            3, 2, 6, 6, 7, 3,
            // Bottom
            4, 5, 1, 1, 0, 4
        };

        s_Data->CubeVAO = VertexArray::Create();
        Ref<VertexBuffer> cubeVB = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
        cubeVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" }
        });
        s_Data->CubeVAO->AddVertexBuffer(cubeVB);

        Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
        s_Data->CubeVAO->SetIndexBuffer(cubeIB);
        
        CE_INFO("Renderer3D: Cube primitive initialized successfully");
    }

}
