#include "ClaudeEngine/Scene/Entity.h"

namespace ClaudeEngine {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene) {
    }

}
