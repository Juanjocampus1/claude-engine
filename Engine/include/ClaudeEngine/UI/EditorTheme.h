#pragma once

#include <imgui.h>

namespace ClaudeEngine {

    class EditorTheme {
    public:
        static void SetDarkTheme();
        static void SetLightTheme();
        static void SetClassicTheme();
        
        // Custom professional dark theme
        static void SetProfessionalDarkTheme();
        
        // Color scheme
        struct Colors {
            ImVec4 Background;
            ImVec4 BackgroundDark;
            ImVec4 BackgroundLight;
            ImVec4 Text;
            ImVec4 TextDisabled;
            ImVec4 Border;
            ImVec4 Accent;
            ImVec4 AccentHovered;
            ImVec4 AccentActive;
            ImVec4 Header;
            ImVec4 HeaderHovered;
            ImVec4 HeaderActive;
        };
        
        static Colors GetCurrentColors();
        static void ApplyColors(const Colors& colors);
    };

}
