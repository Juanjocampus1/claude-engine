#include "ClaudeEngine/Renderer/RenderAPI.h"
#include "ClaudeEngine/Platform/OpenGL/OpenGLRenderAPI.h"

namespace ClaudeEngine {

    RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

    Scope<RenderAPI> RenderAPI::Create() {
        switch (s_API) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateScope<OpenGLRenderAPI>();
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
