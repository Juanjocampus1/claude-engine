#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace ClaudeEngine {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct MeshTexture {
        Ref<Texture2D> Texture;
        std::string Type;
        std::string Path;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, 
             const std::vector<uint32_t>& indices,
             const std::vector<MeshTexture>& textures);

        void Draw(const Ref<Shader>& shader);

        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

    private:
        void SetupMesh();

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<MeshTexture> m_Textures;

        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };

}
