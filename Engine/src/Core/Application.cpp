#include "ClaudeEngine/Core/Application.h"
#include "ClaudeEngine/Core/Log.h"
#include "ClaudeEngine/Renderer/Renderer.h"
#include "ClaudeEngine/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace ClaudeEngine {

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        CE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        Log::Init();
        CE_CORE_INFO("Initializing Claude Engine...");

        m_Window = Window::Create(WindowProps(name));
        m_Window->SetEventCallback(CE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        GLFWwindow* window = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        CE_CORE_INFO("Claude Engine initialized successfully!");
    }

    Application::~Application() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        Renderer::Shutdown();
    }

    void Application::Run() {
        while (m_Running) {
            float time = (float)glfwGetTime();
            float deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized) {
                OnUpdate(deltaTime);
                OnRender();

                // ImGui
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // Enable docking (docking branch API)
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::DockSpaceOverViewport(viewport->ID, viewport);

                OnImGuiRender();

                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                ImGuiIO& io = ImGui::GetIO();
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }
            }

            m_Window->OnUpdate();
        }
    }

    void Application::Close() {
        m_Running = false;
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(CE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(CE_BIND_EVENT_FN(Application::OnWindowResize));
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }

}
