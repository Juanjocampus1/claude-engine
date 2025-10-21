#pragma once

#include "ClaudeEngine/Core/Core.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace ClaudeEngine {

    // Material workflow types
    enum class MaterialWorkflow {
        PBR_MetallicRoughness = 0,  // Standard PBR
        PBR_SpecularGlossiness = 1,  // Alternative PBR
        Unlit = 2,
        RayTracing = 3  // Optimized for ray tracing
    };

    // Material properties for ray tracing
    struct RayTracingProperties {
        // Albedo/Base Color
        glm::vec3 Albedo = { 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> AlbedoMap;
        
        // Metallic-Roughness workflow
        float Metallic = 0.0f;
        float Roughness = 0.5f;
        Ref<Texture2D> MetallicMap;
        Ref<Texture2D> RoughnessMap;
        Ref<Texture2D> MetallicRoughnessMap; // Combined
        
        // Normal mapping
        Ref<Texture2D> NormalMap;
        float NormalStrength = 1.0f;
        
        // Ambient Occlusion
        Ref<Texture2D> AOMap;
        float AOStrength = 1.0f;
        
        // Emission
        glm::vec3 Emission = { 0.0f, 0.0f, 0.0f };
        float EmissionStrength = 0.0f;
        Ref<Texture2D> EmissionMap;
        
        // Advanced RT properties
        float IOR = 1.45f; // Index of Refraction
        float Transmission = 0.0f; // Glass/transparency
        float ClearcoatStrength = 0.0f;
        float ClearcoatRoughness = 0.0f;
        float Sheen = 0.0f;
        glm::vec3 SheenTint = { 1.0f, 1.0f, 1.0f };
        float Anisotropy = 0.0f;
        float AnisotropyRotation = 0.0f;
        
        // Subsurface scattering
        float SubsurfaceStrength = 0.0f;
        glm::vec3 SubsurfaceColor = { 1.0f, 1.0f, 1.0f };
        float SubsurfaceRadius = 1.0f;
    };

    class Material {
    public:
        Material(const std::string& name = "Material", MaterialWorkflow workflow = MaterialWorkflow::PBR_MetallicRoughness);
        ~Material() = default;

        void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }
        Ref<Shader> GetShader() const { return m_Shader; }

        void SetWorkflow(MaterialWorkflow workflow) { m_Workflow = workflow; }
        MaterialWorkflow GetWorkflow() const { return m_Workflow; }

        // Ray tracing properties
        RayTracingProperties& GetRTProperties() { return m_RTProperties; }
        const RayTracingProperties& GetRTProperties() const { return m_RTProperties; }

        // Generic property setters
        void SetFloat(const std::string& name, float value);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetTexture(const std::string& name, const Ref<Texture2D>& texture);

        // Bind material to shader
        void Bind();
        void Unbind();

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

    private:
        std::string m_Name;
        Ref<Shader> m_Shader;
        MaterialWorkflow m_Workflow;
        RayTracingProperties m_RTProperties;

        // Generic property storage
        std::unordered_map<std::string, float> m_FloatProperties;
        std::unordered_map<std::string, glm::vec3> m_Vec3Properties;
        std::unordered_map<std::string, glm::vec4> m_Vec4Properties;
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureProperties;
    };

    // Material Library for managing materials
    class MaterialLibrary {
    public:
        void Add(const std::string& name, const Ref<Material>& material);
        Ref<Material> Load(const std::string& name, MaterialWorkflow workflow = MaterialWorkflow::PBR_MetallicRoughness);
        Ref<Material> Get(const std::string& name);
        bool Exists(const std::string& name) const;

        // Create default materials
        static Ref<Material> CreateDefaultPBR();
        static Ref<Material> CreateDefaultRayTracing();

    private:
        std::unordered_map<std::string, Ref<Material>> m_Materials;
    };

}
