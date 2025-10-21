#pragma once

#include "Core.h"
#include "ClaudeEngine/Events/Event.h"
#include "ClaudeEngine/Events/ApplicationEvent.h"
#include "ClaudeEngine/Platform/Window.h"

namespace ClaudeEngine {

    class Application {
    public:
        Application(const std::string& name = "Claude Engine");
        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);

        inline Window& GetWindow() { return *m_Window; }
        inline static Application& Get() { return *s_Instance; }

    protected:
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;

        static Application* s_Instance;
    };

    // To be defined by client
    Application* CreateApplication();

}
