#include "ClaudeEngine/Renderer/Framebuffer.h"
#include "ClaudeEngine/Platform/OpenGL/OpenGLFramebuffer.h"
#include "ClaudeEngine/Renderer/RenderAPI.h"

namespace ClaudeEngine {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
        switch (RenderAPI::GetAPI()) {
            case RenderAPI::API::None:
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:
                return CreateRef<OpenGLFramebuffer>(spec);
        }

        CE_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }

}
