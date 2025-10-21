#include "ClaudeEngine/Platform/Window.h"
#include "ClaudeEngine/Platform/WindowsWindow.h"

namespace ClaudeEngine {

    Scope<Window> Window::Create(const WindowProps& props) {
        return CreateScope<WindowsWindow>(props);
    }

}
