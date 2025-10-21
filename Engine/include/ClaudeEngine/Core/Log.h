#pragma once

#include "Core.h"
#include <iostream>
#include <sstream>

namespace ClaudeEngine {

    enum class LogLevel {
        Trace = 0,
        Info,
        Warn,
        Error,
        Fatal
    };

    class Log {
    public:
        static void Init();
        
        template<typename... Args>
        static void Trace(Args&&... args) {
            LogMessage(LogLevel::Trace, std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        static void Info(Args&&... args) {
            LogMessage(LogLevel::Info, std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        static void Warn(Args&&... args) {
            LogMessage(LogLevel::Warn, std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        static void Error(Args&&... args) {
            LogMessage(LogLevel::Error, std::forward<Args>(args)...);
        }
        
        template<typename... Args>
        static void Fatal(Args&&... args) {
            LogMessage(LogLevel::Fatal, std::forward<Args>(args)...);
        }

    private:
        template<typename... Args>
        static void LogMessage(LogLevel level, Args&&... args) {
            std::ostringstream oss;
            
            // Add log level prefix
            switch (level) {
                case LogLevel::Trace: oss << "[TRACE] "; break;
                case LogLevel::Info:  oss << "[INFO]  "; break;
                case LogLevel::Warn:  oss << "[WARN]  "; break;
                case LogLevel::Error: oss << "[ERROR] "; break;
                case LogLevel::Fatal: oss << "[FATAL] "; break;
            }
            
            // Concatenate all arguments
            (oss << ... << args);
            
            // Output to console
            if (level >= LogLevel::Error) {
                std::cerr << oss.str() << std::endl;
            } else {
                std::cout << oss.str() << std::endl;
            }
        }
    };

}

// Core logging macros
#define CE_CORE_TRACE(...)    ::ClaudeEngine::Log::Trace(__VA_ARGS__)
#define CE_CORE_INFO(...)     ::ClaudeEngine::Log::Info(__VA_ARGS__)
#define CE_CORE_WARN(...)     ::ClaudeEngine::Log::Warn(__VA_ARGS__)
#define CE_CORE_ERROR(...)    ::ClaudeEngine::Log::Error(__VA_ARGS__)
#define CE_CORE_FATAL(...)    ::ClaudeEngine::Log::Fatal(__VA_ARGS__)

// Client logging macros
#define CE_TRACE(...)         ::ClaudeEngine::Log::Trace(__VA_ARGS__)
#define CE_INFO(...)          ::ClaudeEngine::Log::Info(__VA_ARGS__)
#define CE_WARN(...)          ::ClaudeEngine::Log::Warn(__VA_ARGS__)
#define CE_ERROR(...)         ::ClaudeEngine::Log::Error(__VA_ARGS__)
#define CE_FATAL(...)         ::ClaudeEngine::Log::Fatal(__VA_ARGS__)
