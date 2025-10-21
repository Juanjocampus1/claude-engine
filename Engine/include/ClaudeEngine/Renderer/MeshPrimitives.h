#pragma once

#include "ClaudeEngine/Renderer/Mesh.h"
#include <memory>
#include <glm/glm.hpp>

namespace ClaudeEngine {

    class MeshPrimitives {
    public:
        // Create a cube (1x1x1 centered at origin)
        static std::shared_ptr<Mesh> CreateCube(float size = 1.0f);

        // Create a sphere using UV sphere algorithm
        static std::shared_ptr<Mesh> CreateSphere(float radius = 1.0f, uint32_t segments = 32, uint32_t rings = 16);

        // Create a plane (XZ plane)
        static std::shared_ptr<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f, uint32_t subdivisions = 1);

        // Create a cylinder
        static std::shared_ptr<Mesh> CreateCylinder(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32);

        // Create a cone
        static std::shared_ptr<Mesh> CreateCone(float radius = 0.5f, float height = 1.0f, uint32_t segments = 32);

        // Create a torus
        static std::shared_ptr<Mesh> CreateTorus(float majorRadius = 1.0f, float minorRadius = 0.3f, 
                                                  uint32_t majorSegments = 32, uint32_t minorSegments = 16);

        // Create a capsule
        static std::shared_ptr<Mesh> CreateCapsule(float radius = 0.5f, float height = 1.0f, 
                                                    uint32_t segments = 16, uint32_t rings = 8);

    private:
        // Helper function to calculate tangents
        static void CalculateTangents(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    };

}
