#include "ClaudeEngine/Renderer/Texture.h"
#include "ClaudeEngine/Renderer/Renderer.h"
#include "ClaudeEngine/Platform/OpenGL/OpenGLTexture.h"

namespace ClaudeEngine {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLTexture2D>(width, height);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLTexture2D>(path);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
