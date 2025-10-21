#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

namespace ClaudeEngine {

    // Forward declarations
    class Model;
    class Shader;
    class Material;

    // ========== CORE COMPONENTS ==========

    struct IDComponent {
        uint64_t ID;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(uint64_t id) : ID(id) {}
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    struct TransformComponent {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f }; // Alias for serialization compatibility
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            : Translation(translation), Position(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
        
        void SetPosition(const glm::vec3& pos) {
            Translation = pos;
            Position = pos;
        }
    };

    // ========== RENDERING COMPONENTS ==========

    struct MeshRendererComponent {
        Ref<Model> ModelAsset;
        std::string ModelPath;
        Ref<Material> MaterialOverride;
        bool CastShadows = true;
        bool ReceiveShadows = true;
        
        // Enhanced features
        bool Visible = true;
        bool FrustumCulling = true;
        int LODLevel = 0; // Level of Detail (0 = highest quality)
        
        // Bounding box for culling
        glm::vec3 BoundingBoxMin = { -0.5f, -0.5f, -0.5f };
        glm::vec3 BoundingBoxMax = { 0.5f, 0.5f, 0.5f };
        
        // Outline for selection
        bool ShowOutline = false;
        glm::vec4 OutlineColor = { 1.0f, 0.5f, 0.0f, 1.0f };
        float OutlineWidth = 2.0f;

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent&) = default;
        MeshRendererComponent(const Ref<Model>& model) : ModelAsset(model) {}
        MeshRendererComponent(const std::string& path) : ModelPath(path) {}
        
        // Calculate bounding box from model
        void CalculateBoundingBox();
    };

    // Specific model type components for different formats
    struct FBXModelComponent {
        std::string FilePath;
        Ref<Model> LoadedModel;
        bool IsLoaded = false;

        FBXModelComponent() = default;
        FBXModelComponent(const std::string& path) : FilePath(path) {}
    };

    struct OBJModelComponent {
        std::string FilePath;
        Ref<Model> LoadedModel;
        bool IsLoaded = false;

        OBJModelComponent() = default;
        OBJModelComponent(const std::string& path) : FilePath(path) {}
    };

    struct GLTFModelComponent {
        std::string FilePath;
        Ref<Model> LoadedModel;
        bool IsLoaded = false;

        GLTFModelComponent() = default;
        GLTFModelComponent(const std::string& path) : FilePath(path) {}
    };

    // ========== LIGHT COMPONENTS ==========

    enum class LightType {
        Directional = 0,
        Point = 1,
        Spot = 2,
        Area = 3
    };

    struct LightComponent {
        LightType Type = LightType::Point;
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 1.0f;
        
        // Point & Spot light
        float Range = 10.0f;
        float Attenuation = 1.0f;
        
        // Spot light
        float InnerConeAngle = 30.0f;
        float OuterConeAngle = 45.0f;
        
        // Area light (for ray tracing)
        glm::vec2 AreaSize = { 1.0f, 1.0f };

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

    // ========== CAMERA COMPONENT ==========

    struct CameraComponent {
        bool Primary = true;
        bool FixedAspectRatio = false;
        
        float FOV = 45.0f;
        float AspectRatio = 16.0f / 9.0f;
        float NearClip = 0.1f;
        float FarClip = 1000.0f;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        glm::mat4 GetProjection() const {
            return glm::perspective(glm::radians(FOV), AspectRatio, NearClip, FarClip);
        }
    };

    // ========== SCRIPTING COMPONENT ==========

    struct ScriptComponent {
        std::string ScriptName;
        // Future: Script instance reference

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
        ScriptComponent(const std::string& name) : ScriptName(name) {}
    };

    // ========== PHYSICS COMPONENTS ==========

    struct RigidbodyComponent {
        float Mass = 1.0f;
        float Drag = 0.0f;
        float AngularDrag = 0.05f;
        bool UseGravity = true;
        bool IsKinematic = false;
        
        glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
        glm::vec3 AngularVelocity = { 0.0f, 0.0f, 0.0f };

        RigidbodyComponent() = default;
        RigidbodyComponent(const RigidbodyComponent&) = default;
    };

    enum class ColliderType {
        Box = 0,
        Sphere = 1,
        Capsule = 2,
        Mesh = 3
    };

    struct ColliderComponent {
        ColliderType Type = ColliderType::Box;
        glm::vec3 Size = { 1.0f, 1.0f, 1.0f };
        glm::vec3 Center = { 0.0f, 0.0f, 0.0f };
        bool IsTrigger = false;

        ColliderComponent() = default;
        ColliderComponent(const ColliderComponent&) = default;
    };

}
