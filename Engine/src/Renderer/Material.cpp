#include "ClaudeEngine/Renderer/Material.h"
#include "ClaudeEngine/Core/Log.h"

namespace ClaudeEngine {

    Material::Material(const std::string& name, MaterialWorkflow workflow)
        : m_Name(name), m_Workflow(workflow) {
    }

    void Material::SetFloat(const std::string& name, float value) {
        m_FloatProperties[name] = value;
    }

    void Material::SetVec3(const std::string& name, const glm::vec3& value) {
        m_Vec3Properties[name] = value;
    }

    void Material::SetVec4(const std::string& name, const glm::vec4& value) {
        m_Vec4Properties[name] = value;
    }

    void Material::SetTexture(const std::string& name, const Ref<Texture2D>& texture) {
        m_TextureProperties[name] = texture;
    }

    void Material::Bind() {
        if (!m_Shader) {
            CE_CORE_WARN("Material '", m_Name, "' has no shader!");
            return;
        }

        m_Shader->Bind();

        // Bind ray tracing properties
        auto& rt = m_RTProperties;
        m_Shader->SetFloat3("u_Material.Albedo", rt.Albedo);
        m_Shader->SetFloat("u_Material.Metallic", rt.Metallic);
        m_Shader->SetFloat("u_Material.Roughness", rt.Roughness);
        m_Shader->SetFloat("u_Material.AO", rt.AOStrength);
        m_Shader->SetFloat3("u_Material.Emission", rt.Emission);
        m_Shader->SetFloat("u_Material.EmissionStrength", rt.EmissionStrength);
        m_Shader->SetFloat("u_Material.IOR", rt.IOR);
        m_Shader->SetFloat("u_Material.Transmission", rt.Transmission);

        // Bind textures
        int textureSlot = 0;
        if (rt.AlbedoMap) {
            rt.AlbedoMap->Bind(textureSlot);
            m_Shader->SetInt("u_AlbedoMap", textureSlot++);
            m_Shader->SetInt("u_UseAlbedoMap", 1);
        } else {
            m_Shader->SetInt("u_UseAlbedoMap", 0);
        }

        if (rt.NormalMap) {
            rt.NormalMap->Bind(textureSlot);
            m_Shader->SetInt("u_NormalMap", textureSlot++);
            m_Shader->SetInt("u_UseNormalMap", 1);
        } else {
            m_Shader->SetInt("u_UseNormalMap", 0);
        }

        if (rt.MetallicRoughnessMap) {
            rt.MetallicRoughnessMap->Bind(textureSlot);
            m_Shader->SetInt("u_MetallicRoughnessMap", textureSlot++);
            m_Shader->SetInt("u_UseMetallicRoughnessMap", 1);
        } else {
            m_Shader->SetInt("u_UseMetallicRoughnessMap", 0);
        }

        // Bind generic properties
        for (const auto& [name, value] : m_FloatProperties) {
            m_Shader->SetFloat(name, value);
        }
        for (const auto& [name, value] : m_Vec3Properties) {
            m_Shader->SetFloat3(name, value);
        }
        for (const auto& [name, value] : m_Vec4Properties) {
            m_Shader->SetFloat4(name, value);
        }
    }

    void Material::Unbind() {
        if (m_Shader)
            m_Shader->Unbind();
    }

    // Material Library

    void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material) {
        CE_CORE_ASSERT(!Exists(name), "Material already exists!");
        m_Materials[name] = material;
    }

    Ref<Material> MaterialLibrary::Load(const std::string& name, MaterialWorkflow workflow) {
        auto material = CreateRef<Material>(name, workflow);
        Add(name, material);
        return material;
    }

    Ref<Material> MaterialLibrary::Get(const std::string& name) {
        CE_CORE_ASSERT(Exists(name), "Material not found!");
        return m_Materials[name];
    }

    bool MaterialLibrary::Exists(const std::string& name) const {
        return m_Materials.find(name) != m_Materials.end();
    }

    Ref<Material> MaterialLibrary::CreateDefaultPBR() {
        auto material = CreateRef<Material>("Default PBR", MaterialWorkflow::PBR_MetallicRoughness);
        auto& rt = material->GetRTProperties();
        rt.Albedo = { 0.8f, 0.8f, 0.8f };
        rt.Metallic = 0.0f;
        rt.Roughness = 0.5f;
        return material;
    }

    Ref<Material> MaterialLibrary::CreateDefaultRayTracing() {
        auto material = CreateRef<Material>("Default RT", MaterialWorkflow::RayTracing);
        auto& rt = material->GetRTProperties();
        rt.Albedo = { 0.8f, 0.8f, 0.8f };
        rt.Metallic = 0.0f;
        rt.Roughness = 0.5f;
        rt.IOR = 1.45f;
        return material;
    }

}
