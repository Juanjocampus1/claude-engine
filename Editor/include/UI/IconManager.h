#pragma once

#include "ClaudeEngine/Renderer/Texture.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <imgui.h>
#include <glm/glm.hpp>

namespace ClaudeEngine {

    enum class IconType {
        // File types
        File,
        Folder,
        FolderOpen,
        Image,
        Model,
        Shader,
        Script,
        Audio,
        Scene,
        
        // Components
        Transform,
        Camera,
        Light,
        MeshRenderer,
        
        // Toolbar
        Play,
        Pause,
        Stop,
        Translate,
        Rotate,
        Scale,
        
        // Common
        Add,
        Remove,
        Settings,
        Search,
        Refresh,
        
        Unknown
    };

    class IconManager {
    public:
        static void Init();
        static void Shutdown();
        
        static Ref<Texture2D> GetIcon(IconType type);
        static ImTextureID GetIconImGui(IconType type);
        
        // Load custom icon
        static void LoadIcon(IconType type, const std::string& filepath);
        
        // Get icon size
        static glm::vec2 GetIconSize() { return s_IconSize; }
        static void SetIconSize(const glm::vec2& size) { s_IconSize = size; }
        
    private:
        static void LoadDefaultIcons();
        static Ref<Texture2D> CreateColoredIcon(const glm::vec4& color);
        
    private:
        static std::unordered_map<IconType, Ref<Texture2D>> s_Icons;
        static glm::vec2 s_IconSize;
    };

}
