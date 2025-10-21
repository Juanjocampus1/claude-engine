#pragma once

#include "ClaudeEngine/Core/Core.h"
#include <entt/entt.hpp>
#include <string>
#include <vector>

namespace ClaudeEngine {

    class Entity;

    class Scene {
    public:
        Scene(const std::string& name = "Untitled Scene");
        ~Scene();

        Entity CreateEntity(const std::string& name = "Entity");
        Entity CreateEntityWithUUID(uint64_t uuid, const std::string& name = "Entity");
        void DestroyEntity(Entity entity);

        void OnUpdate(float deltaTime);
        void OnRender();

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        // Get all entities
        entt::registry& GetRegistry() { return m_Registry; }
        
        // Find entities by tag
        Entity FindEntityByTag(const std::string& tag);
        std::vector<Entity> FindEntitiesByTag(const std::string& tag);

    private:
        std::string m_Name;
        entt::registry m_Registry;

        friend class Entity;
    };

}
