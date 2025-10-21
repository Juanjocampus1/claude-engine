#include "ClaudeEngine/Renderer/MeshPrimitives.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ClaudeEngine {

    std::shared_ptr<Mesh> MeshPrimitives::CreateCube(float size) {
        float halfSize = size * 0.5f;
        
        std::vector<Vertex> vertices = {
            // Front face (Z+)
            {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            
            // Back face (Z-)
            {{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            
            // Right face (X+)
            {{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            
            // Left face (X-)
            {{-halfSize, -halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-halfSize, -halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{-halfSize,  halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-halfSize,  halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            
            // Top face (Y+)
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            
            // Bottom face (Y-)
            {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
            {{-halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        };

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Right
            12, 13, 14, 14, 15, 12, // Left
            16, 17, 18, 18, 19, 16, // Top
            20, 21, 22, 22, 23, 20  // Bottom
        };

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreateSphere(float radius, uint32_t segments, uint32_t rings) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        // Generate vertices
        for (uint32_t ring = 0; ring <= rings; ring++) {
            float phi = M_PI * static_cast<float>(ring) / static_cast<float>(rings);
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (uint32_t segment = 0; segment <= segments; segment++) {
                float theta = 2.0f * M_PI * static_cast<float>(segment) / static_cast<float>(segments);
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                Vertex vertex;
                vertex.Normal = glm::vec3(cosTheta * sinPhi, cosPhi, sinTheta * sinPhi);
                vertex.Position = vertex.Normal * radius;
                vertex.TexCoords = glm::vec2(
                    static_cast<float>(segment) / static_cast<float>(segments),
                    static_cast<float>(ring) / static_cast<float>(rings)
                );

                // Tangent calculation (perpendicular to normal in UV space)
                vertex.Tangent = glm::normalize(glm::vec3(-sinTheta, 0.0f, cosTheta));

                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (uint32_t ring = 0; ring < rings; ring++) {
            for (uint32_t segment = 0; segment < segments; segment++) {
                uint32_t current = ring * (segments + 1) + segment;
                uint32_t next = current + segments + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreatePlane(float width, float height, uint32_t subdivisions) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;
        uint32_t vertexCount = subdivisions + 1;

        // Generate vertices
        for (uint32_t z = 0; z < vertexCount; z++) {
            for (uint32_t x = 0; x < vertexCount; x++) {
                float xPos = (static_cast<float>(x) / static_cast<float>(subdivisions)) * width - halfWidth;
                float zPos = (static_cast<float>(z) / static_cast<float>(subdivisions)) * height - halfHeight;

                Vertex vertex;
                vertex.Position = glm::vec3(xPos, 0.0f, zPos);
                vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.TexCoords = glm::vec2(
                    static_cast<float>(x) / static_cast<float>(subdivisions),
                    static_cast<float>(z) / static_cast<float>(subdivisions)
                );
                vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (uint32_t z = 0; z < subdivisions; z++) {
            for (uint32_t x = 0; x < subdivisions; x++) {
                uint32_t topLeft = z * vertexCount + x;
                uint32_t topRight = topLeft + 1;
                uint32_t bottomLeft = (z + 1) * vertexCount + x;
                uint32_t bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreateCylinder(float radius, float height, uint32_t segments) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float halfHeight = height * 0.5f;

        // Top center vertex
        vertices.push_back({{0.0f, halfHeight, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}});
        // Bottom center vertex
        vertices.push_back({{0.0f, -halfHeight, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}});

        // Generate side vertices (top and bottom rings)
        for (uint32_t i = 0; i <= segments; i++) {
            float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            glm::vec3 normal(cosTheta, 0.0f, sinTheta);
            glm::vec3 tangent(-sinTheta, 0.0f, cosTheta);

            // Top ring
            vertices.push_back({
                {cosTheta * radius, halfHeight, sinTheta * radius},
                normal,
                {static_cast<float>(i) / static_cast<float>(segments), 1.0f},
                tangent
            });

            // Bottom ring
            vertices.push_back({
                {cosTheta * radius, -halfHeight, sinTheta * radius},
                normal,
                {static_cast<float>(i) / static_cast<float>(segments), 0.0f},
                tangent
            });
        }

        // Top cap indices
        for (uint32_t i = 0; i < segments; i++) {
            indices.push_back(0);
            indices.push_back(2 + i * 2);
            indices.push_back(2 + (i + 1) * 2);
        }

        // Bottom cap indices
        for (uint32_t i = 0; i < segments; i++) {
            indices.push_back(1);
            indices.push_back(3 + (i + 1) * 2);
            indices.push_back(3 + i * 2);
        }

        // Side indices
        for (uint32_t i = 0; i < segments; i++) {
            uint32_t topCurrent = 2 + i * 2;
            uint32_t bottomCurrent = 3 + i * 2;
            uint32_t topNext = 2 + (i + 1) * 2;
            uint32_t bottomNext = 3 + (i + 1) * 2;

            indices.push_back(topCurrent);
            indices.push_back(bottomCurrent);
            indices.push_back(topNext);

            indices.push_back(topNext);
            indices.push_back(bottomCurrent);
            indices.push_back(bottomNext);
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreateCone(float radius, float height, uint32_t segments) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float halfHeight = height * 0.5f;

        // Apex vertex
        vertices.push_back({{0.0f, halfHeight, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}});
        
        // Bottom center vertex
        vertices.push_back({{0.0f, -halfHeight, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}});

        // Generate bottom ring vertices
        for (uint32_t i = 0; i <= segments; i++) {
            float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            glm::vec3 position(cosTheta * radius, -halfHeight, sinTheta * radius);
            glm::vec3 toApex = glm::normalize(glm::vec3(0.0f, halfHeight, 0.0f) - position);
            glm::vec3 tangent(-sinTheta, 0.0f, cosTheta);
            glm::vec3 normal = glm::normalize(glm::cross(tangent, toApex));

            // Side vertex
            vertices.push_back({
                position,
                normal,
                {static_cast<float>(i) / static_cast<float>(segments), 0.0f},
                tangent
            });
        }

        // Bottom cap indices
        for (uint32_t i = 0; i < segments; i++) {
            indices.push_back(1);
            indices.push_back(3 + i);
            indices.push_back(2 + i);
        }

        // Side indices
        for (uint32_t i = 0; i < segments; i++) {
            indices.push_back(0);
            indices.push_back(2 + i);
            indices.push_back(2 + i + 1);
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreateTorus(float majorRadius, float minorRadius, 
                                                        uint32_t majorSegments, uint32_t minorSegments) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (uint32_t i = 0; i <= majorSegments; i++) {
            float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(majorSegments);
            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);

            for (uint32_t j = 0; j <= minorSegments; j++) {
                float phi = 2.0f * M_PI * static_cast<float>(j) / static_cast<float>(minorSegments);
                float cosPhi = std::cos(phi);
                float sinPhi = std::sin(phi);

                Vertex vertex;
                vertex.Position = glm::vec3(
                    (majorRadius + minorRadius * cosPhi) * cosTheta,
                    minorRadius * sinPhi,
                    (majorRadius + minorRadius * cosPhi) * sinTheta
                );

                glm::vec3 center(majorRadius * cosTheta, 0.0f, majorRadius * sinTheta);
                vertex.Normal = glm::normalize(vertex.Position - center);
                
                vertex.TexCoords = glm::vec2(
                    static_cast<float>(i) / static_cast<float>(majorSegments),
                    static_cast<float>(j) / static_cast<float>(minorSegments)
                );

                vertex.Tangent = glm::normalize(glm::vec3(-sinTheta, 0.0f, cosTheta));

                vertices.push_back(vertex);
            }
        }

        // Generate indices
        for (uint32_t i = 0; i < majorSegments; i++) {
            for (uint32_t j = 0; j < minorSegments; j++) {
                uint32_t current = i * (minorSegments + 1) + j;
                uint32_t next = (i + 1) * (minorSegments + 1) + j;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    std::shared_ptr<Mesh> MeshPrimitives::CreateCapsule(float radius, float height, 
                                                          uint32_t segments, uint32_t rings) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        float cylinderHeight = height - 2.0f * radius;
        float halfCylinderHeight = cylinderHeight * 0.5f;

        // Top hemisphere
        for (uint32_t ring = 0; ring <= rings / 2; ring++) {
            float phi = M_PI * 0.5f * static_cast<float>(ring) / static_cast<float>(rings / 2);
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (uint32_t segment = 0; segment <= segments; segment++) {
                float theta = 2.0f * M_PI * static_cast<float>(segment) / static_cast<float>(segments);
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                Vertex vertex;
                vertex.Normal = glm::vec3(cosTheta * sinPhi, cosPhi, sinTheta * sinPhi);
                vertex.Position = vertex.Normal * radius + glm::vec3(0.0f, halfCylinderHeight, 0.0f);
                vertex.TexCoords = glm::vec2(
                    static_cast<float>(segment) / static_cast<float>(segments),
                    static_cast<float>(ring) / static_cast<float>(rings)
                );
                vertex.Tangent = glm::normalize(glm::vec3(-sinTheta, 0.0f, cosTheta));

                vertices.push_back(vertex);
            }
        }

        uint32_t topHemisphereVertexCount = vertices.size();

        // Bottom hemisphere
        for (uint32_t ring = 0; ring <= rings / 2; ring++) {
            float phi = M_PI * 0.5f + M_PI * 0.5f * static_cast<float>(ring) / static_cast<float>(rings / 2);
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            for (uint32_t segment = 0; segment <= segments; segment++) {
                float theta = 2.0f * M_PI * static_cast<float>(segment) / static_cast<float>(segments);
                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                Vertex vertex;
                vertex.Normal = glm::vec3(cosTheta * sinPhi, cosPhi, sinTheta * sinPhi);
                vertex.Position = vertex.Normal * radius + glm::vec3(0.0f, -halfCylinderHeight, 0.0f);
                vertex.TexCoords = glm::vec2(
                    static_cast<float>(segment) / static_cast<float>(segments),
                    0.5f + static_cast<float>(ring) / static_cast<float>(rings)
                );
                vertex.Tangent = glm::normalize(glm::vec3(-sinTheta, 0.0f, cosTheta));

                vertices.push_back(vertex);
            }
        }

        // Generate indices for top hemisphere
        uint32_t ringsPerHemisphere = rings / 2;
        for (uint32_t ring = 0; ring < ringsPerHemisphere; ring++) {
            for (uint32_t segment = 0; segment < segments; segment++) {
                uint32_t current = ring * (segments + 1) + segment;
                uint32_t next = current + segments + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        // Generate indices for bottom hemisphere
        for (uint32_t ring = 0; ring < ringsPerHemisphere; ring++) {
            for (uint32_t segment = 0; segment < segments; segment++) {
                uint32_t current = topHemisphereVertexCount + ring * (segments + 1) + segment;
                uint32_t next = current + segments + 1;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        std::vector<MeshTexture> textures;
        return std::make_shared<Mesh>(vertices, indices, textures);
    }

    void MeshPrimitives::CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            Vertex& v0 = vertices[indices[i]];
            Vertex& v1 = vertices[indices[i + 1]];
            Vertex& v2 = vertices[indices[i + 2]];

            glm::vec3 edge1 = v1.Position - v0.Position;
            glm::vec3 edge2 = v2.Position - v0.Position;

            glm::vec2 deltaUV1 = v1.TexCoords - v0.TexCoords;
            glm::vec2 deltaUV2 = v2.TexCoords - v0.TexCoords;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            v0.Tangent += tangent;
            v1.Tangent += tangent;
            v2.Tangent += tangent;
        }

        for (auto& vertex : vertices) {
            vertex.Tangent = glm::normalize(vertex.Tangent);
        }
    }

}
