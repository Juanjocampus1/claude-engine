#include "ClaudeEngine/Scene/SceneSerializer.h"
#include "ClaudeEngine/Scene/Entity.h"
#include "ClaudeEngine/Scene/Components.h"
#include "ClaudeEngine/Core/Log.h"
#include "ClaudeEngine/Renderer/Camera.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML {

    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

}

namespace ClaudeEngine {

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
        : m_Scene(scene) {
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity) {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << static_cast<uint32_t>(entity);

        if (entity.HasComponent<TagComponent>()) {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap; // TagComponent
        }

        if (entity.HasComponent<TransformComponent>()) {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << tc.Position;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap; // TransformComponent
        }

        if (entity.HasComponent<CameraComponent>()) {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent

            auto& cc = entity.GetComponent<CameraComponent>();
            out << YAML::Key << "FOV" << YAML::Value << cc.FOV;
            out << YAML::Key << "AspectRatio" << YAML::Value << cc.AspectRatio;
            out << YAML::Key << "NearClip" << YAML::Value << cc.NearClip;
            out << YAML::Key << "FarClip" << YAML::Value << cc.FarClip;
            out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

            out << YAML::EndMap; // CameraComponent
        }

        if (entity.HasComponent<MeshRendererComponent>()) {
            out << YAML::Key << "MeshRendererComponent";
            out << YAML::BeginMap; // MeshRendererComponent

            auto& mrc = entity.GetComponent<MeshRendererComponent>();
            out << YAML::Key << "ModelPath" << YAML::Value << mrc.ModelPath;
            out << YAML::Key << "CastShadows" << YAML::Value << mrc.CastShadows;
            out << YAML::Key << "ReceiveShadows" << YAML::Value << mrc.ReceiveShadows;

            out << YAML::EndMap; // MeshRendererComponent
        }

        if (entity.HasComponent<LightComponent>()) {
            out << YAML::Key << "LightComponent";
            out << YAML::BeginMap; // LightComponent

            auto& lc = entity.GetComponent<LightComponent>();
            out << YAML::Key << "Type" << YAML::Value << static_cast<int>(lc.Type);
            out << YAML::Key << "Color" << YAML::Value << lc.Color;
            out << YAML::Key << "Intensity" << YAML::Value << lc.Intensity;
            out << YAML::Key << "Range" << YAML::Value << lc.Range;

            out << YAML::EndMap; // LightComponent
        }

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::Serialize(const std::string& filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        // Use view<> to iterate all entities (EnTT v3.x API)
        auto view = m_Scene->GetRegistry().view<TagComponent>();
        for (auto entityID : view) {
            Entity entity = { entityID, m_Scene.get() };
            if (!entity)
                continue;

            SerializeEntity(out, entity);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
        fout.close();

        CE_INFO("Scene saved to: {0}", filepath);
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath) {
        // TODO: Implement binary serialization for runtime
        CE_ASSERT(false, "Not implemented!");
    }

    bool SceneSerializer::Deserialize(const std::string& filepath) {
        std::ifstream stream(filepath);
        if (!stream.is_open()) {
            CE_ERROR("Failed to open scene file: {0}", filepath);
            return false;
        }

        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data;
        try {
            data = YAML::Load(strStream.str());
        }
        catch (YAML::ParserException e) {
            CE_ERROR("Failed to load scene file '{0}'\n     {1}", filepath, e.what());
            return false;
        }

        if (!data["Scene"]) {
            CE_ERROR("Scene file is missing 'Scene' node!");
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();
        CE_INFO("Deserializing scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();

                CE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

                Entity deserializedEntity = m_Scene->CreateEntity(name);

                auto transformComponent = entity["TransformComponent"];
                if (transformComponent) {
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Position = transformComponent["Position"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent) {
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>();
                    cc.FOV = cameraComponent["FOV"].as<float>();
                    cc.AspectRatio = cameraComponent["AspectRatio"].as<float>();
                    cc.NearClip = cameraComponent["NearClip"].as<float>();
                    cc.FarClip = cameraComponent["FarClip"].as<float>();
                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }

                auto meshRendererComponent = entity["MeshRendererComponent"];
                if (meshRendererComponent) {
                    auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
                    mrc.ModelPath = meshRendererComponent["ModelPath"].as<std::string>();
                    mrc.CastShadows = meshRendererComponent["CastShadows"].as<bool>();
                    mrc.ReceiveShadows = meshRendererComponent["ReceiveShadows"].as<bool>();
                }

                auto lightComponent = entity["LightComponent"];
                if (lightComponent) {
                    auto& lc = deserializedEntity.AddComponent<LightComponent>();
                    lc.Type = static_cast<LightType>(lightComponent["Type"].as<int>());
                    lc.Color = lightComponent["Color"].as<glm::vec3>();
                    lc.Intensity = lightComponent["Intensity"].as<float>();
                    lc.Range = lightComponent["Range"].as<float>();
                }
            }
        }

        CE_INFO("Scene loaded from: {0}", filepath);
        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
        // TODO: Implement binary deserialization for runtime
        CE_ASSERT(false, "Not implemented!");
        return false;
    }

}
