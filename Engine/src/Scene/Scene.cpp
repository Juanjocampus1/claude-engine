#include "ClaudeEngine/Scene/Scene.h"
#include "ClaudeEngine/Scene/Entity.h"
#include "ClaudeEngine/Scene/Components.h"
#include "ClaudeEngine/Core/Log.h"
#include <random>

namespace ClaudeEngine {

    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    Scene::Scene(const std::string& name)
        : m_Name(name) {
        CE_CORE_INFO("Creating scene: ", name);
    }

    Scene::~Scene() {
    }

    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithUUID(s_UniformDistribution(s_Engine), name);
    }

    Entity Scene::CreateEntityWithUUID(uint64_t uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TagComponent>(name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }

    void Scene::OnUpdate(float deltaTime) {
        // Update scripts
        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view) {
                // TODO: Call script update
            }
        }

        // Update physics
        {
            auto view = m_Registry.view<RigidbodyComponent, TransformComponent>();
            for (auto entity : view) {
                auto& rb = view.get<RigidbodyComponent>(entity);
                auto& transform = view.get<TransformComponent>(entity);

                if (!rb.IsKinematic && rb.UseGravity) {
                    rb.Velocity.y -= 9.81f * deltaTime;
                }

                transform.Translation += rb.Velocity * deltaTime;
            }
        }
    }

    void Scene::OnRender() {
        // Rendering is handled by the renderer system
    }

    Entity Scene::FindEntityByTag(const std::string& tag) {
        auto view = m_Registry.view<TagComponent>();
        for (auto entity : view) {
            const TagComponent& tc = view.get<TagComponent>(entity);
            if (tc.Tag == tag)
                return Entity{ entity, this };
        }
        return Entity{}; // Return invalid entity
    }

    std::vector<Entity> Scene::FindEntitiesByTag(const std::string& tag) {
        std::vector<Entity> result;
        auto view = m_Registry.view<TagComponent>();
        for (auto entity : view) {
            const TagComponent& tc = view.get<TagComponent>(entity);
            if (tc.Tag.find(tag) != std::string::npos)
                result.push_back(Entity{ entity, this });
        }
        return result;
    }

}
