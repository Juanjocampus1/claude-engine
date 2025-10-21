#include "ClaudeEngine/Platform/WindowsWindow.h"
#include "ClaudeEngine/Events/ApplicationEvent.h"
#include "ClaudeEngine/Core/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ClaudeEngine {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        CE_CORE_ERROR("GLFW Error (", error, "): ", description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        CE_CORE_INFO("Creating window ", props.Title, " (", props.Width, ", ", props.Height, ")");

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            CE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CE_CORE_ASSERT(status, "Failed to initialize Glad!");

        CE_CORE_INFO("OpenGL Info:");
        CE_CORE_INFO("  Vendor: ", (const char*)glGetString(GL_VENDOR));
        CE_CORE_INFO("  Renderer: ", (const char*)glGetString(GL_RENDERER));
        CE_CORE_INFO("  Version: ", (const char*)glGetString(GL_VERSION));

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const {
        return m_Data.VSync;
    }

}
