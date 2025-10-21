#include "UI/IconManager.h"
#include "ClaudeEngine/Core/Log.h"

namespace ClaudeEngine {

    std::unordered_map<IconType, Ref<Texture2D>> IconManager::s_Icons;
    glm::vec2 IconManager::s_IconSize = { 16.0f, 16.0f };

    void IconManager::Init() {
        CE_INFO("IconManager: Initializing...");
        LoadDefaultIcons();
        CE_INFO("IconManager: Initialized with {0} icons", s_Icons.size());
    }

    void IconManager::Shutdown() {
        CE_INFO("IconManager: Shutting down...");
        s_Icons.clear();
    }

    Ref<Texture2D> IconManager::GetIcon(IconType type) {
        auto it = s_Icons.find(type);
        if (it != s_Icons.end()) {
            return it->second;
        }
        
        // Return unknown icon as fallback
        auto unknownIt = s_Icons.find(IconType::Unknown);
        if (unknownIt != s_Icons.end()) {
            return unknownIt->second;
        }
        
        return nullptr;
    }

    ImTextureID IconManager::GetIconImGui(IconType type) {
        auto icon = GetIcon(type);
        if (icon) {
            return (ImTextureID)(intptr_t)icon->GetRendererID();
        }
        return (ImTextureID)0;
    }

    void IconManager::LoadIcon(IconType type, const std::string& filepath) {
        try {
            auto texture = Texture2D::Create(filepath);
            s_Icons[type] = texture;
            CE_INFO("IconManager: Loaded icon from {0}", filepath);
        } catch (const std::exception& e) {
            CE_ERROR("IconManager: Failed to load icon from {0}: {1}", filepath, e.what());
        }
    }

    Ref<Texture2D> IconManager::CreateColoredIcon(const glm::vec4& color) {
        // Create a simple 16x16 colored texture as placeholder
        uint32_t width = 16;
        uint32_t height = 16;
        uint32_t* data = new uint32_t[width * height];
        
        uint8_t r = static_cast<uint8_t>(color.r * 255);
        uint8_t g = static_cast<uint8_t>(color.g * 255);
        uint8_t b = static_cast<uint8_t>(color.b * 255);
        uint8_t a = static_cast<uint8_t>(color.a * 255);
        
        uint32_t packedColor = (a << 24) | (b << 16) | (g << 8) | r;
        
        for (uint32_t i = 0; i < width * height; i++) {
            data[i] = packedColor;
        }
        
        auto texture = Texture2D::Create(width, height);
        texture->SetData(data, width * height * sizeof(uint32_t));
        
        delete[] data;
        return texture;
    }

    void IconManager::LoadDefaultIcons() {
        // Try to load icons from files, fallback to colored placeholders
        
        // File types (Orange shades)
        s_Icons[IconType::File] = CreateColoredIcon({ 0.8f, 0.8f, 0.8f, 1.0f });
        s_Icons[IconType::Folder] = CreateColoredIcon({ 0.9f, 0.7f, 0.2f, 1.0f });
        s_Icons[IconType::FolderOpen] = CreateColoredIcon({ 1.0f, 0.8f, 0.3f, 1.0f });
        s_Icons[IconType::Image] = CreateColoredIcon({ 0.4f, 0.8f, 0.4f, 1.0f });
        s_Icons[IconType::Model] = CreateColoredIcon({ 0.3f, 0.7f, 0.9f, 1.0f });
        s_Icons[IconType::Shader] = CreateColoredIcon({ 0.9f, 0.3f, 0.7f, 1.0f });
        s_Icons[IconType::Script] = CreateColoredIcon({ 0.7f, 0.9f, 0.3f, 1.0f });
        s_Icons[IconType::Audio] = CreateColoredIcon({ 0.8f, 0.4f, 0.8f, 1.0f });
        s_Icons[IconType::Scene] = CreateColoredIcon({ 0.5f, 0.7f, 0.9f, 1.0f });
        
        // Components (Blue shades)
        s_Icons[IconType::Transform] = CreateColoredIcon({ 0.3f, 0.5f, 0.9f, 1.0f });
        s_Icons[IconType::Camera] = CreateColoredIcon({ 0.2f, 0.6f, 0.8f, 1.0f });
        s_Icons[IconType::Light] = CreateColoredIcon({ 0.9f, 0.9f, 0.5f, 1.0f });
        s_Icons[IconType::MeshRenderer] = CreateColoredIcon({ 0.4f, 0.6f, 0.9f, 1.0f });
        
        // Toolbar (Various colors)
        s_Icons[IconType::Play] = CreateColoredIcon({ 0.3f, 0.9f, 0.3f, 1.0f });
        s_Icons[IconType::Pause] = CreateColoredIcon({ 0.9f, 0.7f, 0.2f, 1.0f });
        s_Icons[IconType::Stop] = CreateColoredIcon({ 0.9f, 0.3f, 0.3f, 1.0f });
        s_Icons[IconType::Translate] = CreateColoredIcon({ 0.9f, 0.3f, 0.3f, 1.0f });
        s_Icons[IconType::Rotate] = CreateColoredIcon({ 0.3f, 0.9f, 0.3f, 1.0f });
        s_Icons[IconType::Scale] = CreateColoredIcon({ 0.3f, 0.3f, 0.9f, 1.0f });
        
        // Common (Gray shades)
        s_Icons[IconType::Add] = CreateColoredIcon({ 0.4f, 0.9f, 0.4f, 1.0f });
        s_Icons[IconType::Remove] = CreateColoredIcon({ 0.9f, 0.4f, 0.4f, 1.0f });
        s_Icons[IconType::Settings] = CreateColoredIcon({ 0.7f, 0.7f, 0.7f, 1.0f });
        s_Icons[IconType::Search] = CreateColoredIcon({ 0.6f, 0.8f, 0.9f, 1.0f });
        s_Icons[IconType::Refresh] = CreateColoredIcon({ 0.5f, 0.8f, 0.5f, 1.0f });
        
        // Unknown
        s_Icons[IconType::Unknown] = CreateColoredIcon({ 0.5f, 0.5f, 0.5f, 1.0f });
        
        // Try to load from actual icon files if they exist
        // These will fail silently and keep the colored placeholders
        LoadIcon(IconType::Folder, "assets/icons/folder.png");
        LoadIcon(IconType::FolderOpen, "assets/icons/folder_open.png");
        LoadIcon(IconType::File, "assets/icons/file.png");
    }

}
