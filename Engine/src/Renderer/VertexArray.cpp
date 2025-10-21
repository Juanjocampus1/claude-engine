#include "ClaudeEngine/Renderer/VertexArray.h"
#include "ClaudeEngine/Renderer/Renderer.h"
#include "ClaudeEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace ClaudeEngine {

    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLVertexArray>();
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
