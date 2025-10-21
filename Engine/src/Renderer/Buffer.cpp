#include "ClaudeEngine/Renderer/Buffer.h"
#include "ClaudeEngine/Renderer/Renderer.h"
#include "ClaudeEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace ClaudeEngine {

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLVertexBuffer>(size);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RenderAPI::API::None:    
                CE_CORE_ASSERT(false, "RenderAPI::None is not supported!");
                return nullptr;
            case RenderAPI::API::OpenGL:  
                return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
