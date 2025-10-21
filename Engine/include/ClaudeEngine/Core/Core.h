#pragma once

#include <memory>
#include <string>

// Platform detection
#ifdef _WIN32
    #define CE_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define CE_PLATFORM_LINUX
#elif defined(__APPLE__)
    #define CE_PLATFORM_MACOS
#endif

// Build configuration
#ifdef CE_DEBUG
    #define CE_ENABLE_ASSERTS
    #define CE_ENABLE_LOGGING
#endif

// Assertions
#ifdef CE_ENABLE_ASSERTS
    #include <cassert>
    #define CE_ASSERT(x, msg) assert(x && msg)
    #define CE_CORE_ASSERT(x, msg) assert(x && msg)
#else
    #define CE_ASSERT(x, msg)
    #define CE_CORE_ASSERT(x, msg)
#endif

// Smart pointers
namespace ClaudeEngine {
    template<typename T>
    using Ref = std::shared_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using Scope = std::unique_ptr<T>;
    
    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

// Bit operations
#define BIT(x) (1 << x)

// Bind function helper
#define CE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
