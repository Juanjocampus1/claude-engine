#include "ClaudeEngine/UI/EditorTheme.h"

namespace ClaudeEngine {

    void EditorTheme::SetDarkTheme() {
        ImGui::StyleColorsDark();
    }

    void EditorTheme::SetLightTheme() {
        ImGui::StyleColorsLight();
    }

    void EditorTheme::SetClassicTheme() {
        ImGui::StyleColorsClassic();
    }

    void EditorTheme::SetProfessionalDarkTheme() {
        ImGuiStyle& style = ImGui::GetStyle();

        // Rounding
        style.WindowRounding = 5.0f;
        style.FrameRounding = 4.0f;
        style.GrabRounding = 3.0f;
        style.ScrollbarRounding = 3.0f;
        style.TabRounding = 4.0f;

        // Spacing
        style.WindowPadding = ImVec2(10, 10);
        style.FramePadding = ImVec2(8, 4);
        style.ItemSpacing = ImVec2(8, 4);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.IndentSpacing = 20.0f;

        // Borders
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.TabBorderSize = 0.0f;

        // Sizes
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 10.0f;

        // Colors - Professional Dark Theme (Inspired by Visual Studio Dark)
        ImVec4* colors = style.Colors;
        
        // Window
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frame (inputs, buttons, etc)
        colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.31f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

        // Menu
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Check/Radio
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);

        // Slider
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.58f, 0.90f, 1.00f);

        // Button
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.58f, 0.90f, 1.00f);

        // Header
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.48f, 0.80f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);

        // Separator
        colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.48f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);

        // Resize grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.48f, 0.80f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.48f, 0.80f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.48f, 0.80f, 0.95f);

        // Tab
        colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.48f, 0.80f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.48f, 0.80f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);

        // Plot
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

        // Table
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

        // Text
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.48f, 0.80f, 0.35f);

        // Drag Drop
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

        // Nav
        colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.48f, 0.80f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

        // Modal
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    }

    EditorTheme::Colors EditorTheme::GetCurrentColors() {
        ImGuiStyle& style = ImGui::GetStyle();
        Colors colors;
        
        colors.Background = style.Colors[ImGuiCol_WindowBg];
        colors.BackgroundDark = style.Colors[ImGuiCol_ChildBg];
        colors.BackgroundLight = style.Colors[ImGuiCol_PopupBg];
        colors.Text = style.Colors[ImGuiCol_Text];
        colors.TextDisabled = style.Colors[ImGuiCol_TextDisabled];
        colors.Border = style.Colors[ImGuiCol_Border];
        colors.Accent = style.Colors[ImGuiCol_ButtonHovered];
        colors.AccentHovered = style.Colors[ImGuiCol_ButtonHovered];
        colors.AccentActive = style.Colors[ImGuiCol_ButtonActive];
        colors.Header = style.Colors[ImGuiCol_Header];
        colors.HeaderHovered = style.Colors[ImGuiCol_HeaderHovered];
        colors.HeaderActive = style.Colors[ImGuiCol_HeaderActive];
        
        return colors;
    }

    void EditorTheme::ApplyColors(const Colors& colors) {
        ImGuiStyle& style = ImGui::GetStyle();
        
        style.Colors[ImGuiCol_WindowBg] = colors.Background;
        style.Colors[ImGuiCol_ChildBg] = colors.BackgroundDark;
        style.Colors[ImGuiCol_PopupBg] = colors.BackgroundLight;
        style.Colors[ImGuiCol_Text] = colors.Text;
        style.Colors[ImGuiCol_TextDisabled] = colors.TextDisabled;
        style.Colors[ImGuiCol_Border] = colors.Border;
        style.Colors[ImGuiCol_Button] = colors.Accent;
        style.Colors[ImGuiCol_ButtonHovered] = colors.AccentHovered;
        style.Colors[ImGuiCol_ButtonActive] = colors.AccentActive;
        style.Colors[ImGuiCol_Header] = colors.Header;
        style.Colors[ImGuiCol_HeaderHovered] = colors.HeaderHovered;
        style.Colors[ImGuiCol_HeaderActive] = colors.HeaderActive;
    }

}
