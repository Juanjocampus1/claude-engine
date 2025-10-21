#include <ClaudeEngine/ClaudeEngine.h>
#include <ClaudeEngine/Scene/Scene.h>
#include <ClaudeEngine/Scene/Entity.h>
#include <ClaudeEngine/Scene/Components.h>
#include <ClaudeEngine/Scene/SceneSerializer.h>
#include <ClaudeEngine/Renderer/Material.h>
#include <ClaudeEngine/Renderer/ModelLoader.h>
#include <ClaudeEngine/Renderer/MeshPrimitives.h>
#include <ClaudeEngine/Renderer/Renderer3D.h>
#include <ClaudeEngine/UI/EditorTheme.h>
#include "Panels/EditorPanels.h"
#include "Panels/ViewportPanel.h"
#include "UI/IconManager.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Primitive types enum
namespace ClaudeEngine {
    enum class PrimitiveType {
        Cube,
        Sphere,
        Plane,
        Cylinder,
        Cone,
        Torus,
        Capsule
    };
}

class EditorLayer {
public:
    EditorLayer() {
        CE_INFO("=== Initializing Editor ===");
        
        try {
            // Initialize Renderer3D
            CE_INFO("Initializing Renderer3D...");
            ClaudeEngine::Renderer3D::Init();
            
            // Initialize IconManager
            CE_INFO("Initializing IconManager...");
            ClaudeEngine::IconManager::Init();
            
            // Create scene
            CE_INFO("Creating scene...");
            m_ActiveScene = ClaudeEngine::CreateRef<ClaudeEngine::Scene>("Main Scene");
            
            // Create camera entity
            CE_INFO("Creating camera entity...");
            m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
            auto& cameraComp = m_CameraEntity.AddComponent<ClaudeEngine::CameraComponent>();
            cameraComp.Primary = true;
            cameraComp.FOV = 45.0f;
            
            auto& cameraTransform = m_CameraEntity.GetComponent<ClaudeEngine::TransformComponent>();
            cameraTransform.Translation = { 0.0f, 2.0f, 10.0f };
            
            // Create legacy camera for now
            CE_INFO("Creating legacy camera...");
            m_Camera = ClaudeEngine::CreateScope<ClaudeEngine::PerspectiveCamera>(
                45.0f, 16.0f / 9.0f, 0.1f, 1000.0f
            );
            m_Camera->SetPosition({ 0.0f, 2.0f, 10.0f });
            m_Camera->SetRotation({ 0.0f, 0.0f, 0.0f });

            // Try to load shader from file (optional, fallback to simple shader)
            CE_INFO("Loading shader...");
            try {
                m_Shader = ClaudeEngine::Shader::Create("assets/shaders/PBR_RayTracing.glsl");
                CE_INFO("Shader loaded successfully from file");
            } catch (...) {
                CE_WARN("Could not load shader from file, using simple shader");
                CreateFallbackShader();
            }
            
            // Create material library
            CE_INFO("Creating material library...");
            m_MaterialLibrary = ClaudeEngine::CreateRef<ClaudeEngine::MaterialLibrary>();
            
            // Create default material
            CE_INFO("Creating default material...");
            m_DefaultMaterial = ClaudeEngine::MaterialLibrary::CreateDefaultRayTracing();
            if (m_Shader) {
                m_DefaultMaterial->SetShader(m_Shader);
            }
            
            // Setup panels
            CE_INFO("Setting up editor panels...");
            m_HierarchyPanel = ClaudeEngine::CreateScope<ClaudeEngine::HierarchyPanel>(m_ActiveScene);
            m_ContentBrowser = ClaudeEngine::CreateScope<ClaudeEngine::ContentBrowserPanel>();
            m_PropertiesPanel = ClaudeEngine::CreateScope<ClaudeEngine::PropertiesPanel>();
            m_SettingsPanel = ClaudeEngine::CreateScope<ClaudeEngine::SettingsPanel>();
            m_StatsPanel = ClaudeEngine::CreateScope<ClaudeEngine::StatsPanel>();
            m_ViewportPanel = ClaudeEngine::CreateScope<ClaudeEngine::ViewportPanel>();
            
            // Create framebuffer for viewport
            CE_INFO("Creating framebuffer...");
            ClaudeEngine::FramebufferSpecification fbSpec;
            fbSpec.Width = 1280;
            fbSpec.Height = 720;
            fbSpec.Attachments = { ClaudeEngine::FramebufferTextureFormat::RGBA8, ClaudeEngine::FramebufferTextureFormat::DEPTH24STENCIL8 };
            m_Framebuffer = ClaudeEngine::Framebuffer::Create(fbSpec);
            m_ViewportPanel->SetFramebuffer(m_Framebuffer);
            
            // Create default entities
            CE_INFO("Creating default scene...");
            CreateDefaultScene();
            
            CE_INFO("=== Editor initialized successfully ===");
        } catch (const std::exception& e) {
            CE_CORE_ERROR("Failed to initialize editor: {0}", e.what());
            throw;
        } catch (...) {
            CE_CORE_ERROR("Failed to initialize editor: Unknown error");
            throw;
        }
    }
    
    ~EditorLayer() {
        CE_INFO("Shutting down editor systems...");
        ClaudeEngine::IconManager::Shutdown();
        ClaudeEngine::Renderer3D::Shutdown();
    }
    
    void CreateFallbackShader() {
        CE_INFO("Creating fallback shader...");
        std::string vertexSrc = R"(
            #version 460 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Normal;
            layout(location = 2) in vec2 a_TexCoords;
            
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;
            
            out vec3 v_Normal;
            out vec2 v_TexCoords;
            out vec3 v_FragPos;
            
            void main() {
                v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
                v_TexCoords = a_TexCoords;
                v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 460 core
            
            layout(location = 0) out vec4 color;
            
            in vec3 v_Normal;
            in vec2 v_TexCoords;
            in vec3 v_FragPos;
            
            uniform vec3 u_LightPos;
            uniform vec3 u_ViewPos;
            uniform vec3 u_ObjectColor;
            
            void main() {
                // Ambient
                float ambientStrength = 0.3;
                vec3 ambient = ambientStrength * vec3(1.0);
                
                // Diffuse
                vec3 norm = normalize(v_Normal);
                vec3 lightDir = normalize(u_LightPos - v_FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * vec3(1.0);
                
                // Specular
                float specularStrength = 0.5;
                vec3 viewDir = normalize(u_ViewPos - v_FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * vec3(1.0);
                
                vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
                color = vec4(result, 1.0);
            }
        )";
        
        m_Shader = ClaudeEngine::Shader::Create("BasicShader", vertexSrc, fragmentSrc);
        CE_INFO("Fallback shader created successfully");
    }
    
    void CreateDefaultScene() {
        // Create a directional light
        auto lightEntity = m_ActiveScene->CreateEntity("Directional Light");
        auto& light = lightEntity.AddComponent<ClaudeEngine::LightComponent>();
        light.Type = ClaudeEngine::LightType::Directional;
        light.Color = { 1.0f, 1.0f, 1.0f };
        light.Intensity = 1.0f;
        
        auto& lightTransform = lightEntity.GetComponent<ClaudeEngine::TransformComponent>();
        lightTransform.Rotation = glm::radians(glm::vec3(-45.0f, -30.0f, 0.0f));
    }

    void OnUpdate(float deltaTime) {
        // Update viewport panel (includes editor camera and scene rendering)
        if (m_ViewportPanel) {
            m_ViewportPanel->SetContext(m_ActiveScene);
            
            // Update selected entity from hierarchy panel
            if (m_HierarchyPanel) {
                m_ViewportPanel->SetSelectedEntity(m_HierarchyPanel->GetSelectedEntity());
            }
            
            m_ViewportPanel->OnUpdate(deltaTime);
        }

        // Gizmo hotkeys
        if (ImGui::IsKeyPressed(ImGuiKey_W)) {
            m_ViewportPanel->SetGizmoOperation(ClaudeEngine::GizmoOperation::Translate);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_E)) {
            m_ViewportPanel->SetGizmoOperation(ClaudeEngine::GizmoOperation::Rotate);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_R)) {
            m_ViewportPanel->SetGizmoOperation(ClaudeEngine::GizmoOperation::Scale);
        }

        // Update rotation
        m_Rotation += 0.5f * deltaTime;
    }

    void OnEvent(ClaudeEngine::Event& e) {
        // Forward events to viewport panel for camera control
        if (m_ViewportPanel) {
            m_ViewportPanel->OnEvent(e);
        }
    }

    void OnRender() {
        // Render to framebuffer
        if (m_Framebuffer) {
            m_Framebuffer->Bind();
        }

        ClaudeEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
        ClaudeEngine::RenderCommand::Clear();

        ClaudeEngine::Renderer::BeginScene(*m_Camera);

        // TODO: Render scene using ECS
        // For now, just setup render state
        if (m_Shader) {
            m_Shader->Bind();
            m_Shader->SetFloat3("u_LightPos", { 5.0f, 5.0f, 5.0f });
            m_Shader->SetFloat3("u_ViewPos", m_Camera->GetPosition());
            m_Shader->SetMat4("u_ViewProjection", m_Camera->GetViewProjection());
        }

        ClaudeEngine::Renderer::EndScene();

        if (m_Framebuffer) {
            m_Framebuffer->Unbind();
        }
    }

    void OnImGuiRender() {
        // Docking space is automatically created by Application

        // Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                    NewScene();
                }
                if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                    SaveScene();
                }
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
                    SaveSceneAs();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4")) {
                    ClaudeEngine::Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("GameObject")) {
                if (ImGui::MenuItem("Create Empty")) {
                    m_ActiveScene->CreateEntity("Empty Entity");
                }
                if (ImGui::BeginMenu("3D Object")) {
                    if (ImGui::MenuItem("Cube")) {
                        CreatePrimitive("Cube", ClaudeEngine::PrimitiveType::Cube);
                    }
                    if (ImGui::MenuItem("Sphere")) {
                        CreatePrimitive("Sphere", ClaudeEngine::PrimitiveType::Sphere);
                    }
                    if (ImGui::MenuItem("Plane")) {
                        CreatePrimitive("Plane", ClaudeEngine::PrimitiveType::Plane);
                    }
                    if (ImGui::MenuItem("Cylinder")) {
                        CreatePrimitive("Cylinder", ClaudeEngine::PrimitiveType::Cylinder);
                    }
                    if (ImGui::MenuItem("Cone")) {
                        CreatePrimitive("Cone", ClaudeEngine::PrimitiveType::Cone);
                    }
                    if (ImGui::MenuItem("Torus")) {
                        CreatePrimitive("Torus", ClaudeEngine::PrimitiveType::Torus);
                    }
                    if (ImGui::MenuItem("Capsule")) {
                        CreatePrimitive("Capsule", ClaudeEngine::PrimitiveType::Capsule);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Light")) {
                    if (ImGui::MenuItem("Directional Light")) {
                        auto entity = m_ActiveScene->CreateEntity("Directional Light");
                        auto& light = entity.AddComponent<ClaudeEngine::LightComponent>();
                        light.Type = ClaudeEngine::LightType::Directional;
                    }
                    if (ImGui::MenuItem("Point Light")) {
                        auto entity = m_ActiveScene->CreateEntity("Point Light");
                        auto& light = entity.AddComponent<ClaudeEngine::LightComponent>();
                        light.Type = ClaudeEngine::LightType::Point;
                    }
                    if (ImGui::MenuItem("Spot Light")) {
                        auto entity = m_ActiveScene->CreateEntity("Spot Light");
                        auto& light = entity.AddComponent<ClaudeEngine::LightComponent>();
                        light.Type = ClaudeEngine::LightType::Spot;
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Camera")) {
                    auto entity = m_ActiveScene->CreateEntity("Camera");
                    entity.AddComponent<ClaudeEngine::CameraComponent>();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window")) {
                ImGui::MenuItem("Hierarchy", nullptr, &m_ShowHierarchy);
                ImGui::MenuItem("Content Browser", nullptr, &m_ShowContentBrowser);
                ImGui::MenuItem("Properties", nullptr, &m_ShowProperties);
                ImGui::MenuItem("Settings", nullptr, &m_ShowSettings);
                ImGui::MenuItem("Stats", nullptr, &m_ShowStats);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {
                    m_ShowAbout = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Viewport (always show)
        m_ViewportPanel->OnImGuiRender();

        // Render panels
        if (m_ShowHierarchy) {
            m_HierarchyPanel->OnImGuiRender();
            m_PropertiesPanel->SetSelectedEntity(m_HierarchyPanel->GetSelectedEntity());
        }
        
        if (m_ShowContentBrowser)
            m_ContentBrowser->OnImGuiRender();
        
        if (m_ShowProperties)
            m_PropertiesPanel->OnImGuiRender();
        
        if (m_ShowSettings)
            m_SettingsPanel->OnImGuiRender();
        
        if (m_ShowStats)
            m_StatsPanel->OnImGuiRender();

        // About dialog
        if (m_ShowAbout) {
            ImGui::OpenPopup("About");
            if (ImGui::BeginPopupModal("About", &m_ShowAbout, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Claude Engine v1.0");
                ImGui::Text("A modular graphics engine built with OpenGL and C++");
                ImGui::Separator();
                ImGui::Text("Features:");
                ImGui::BulletText("OpenGL 4.6 Rendering");
                ImGui::BulletText("Assimp Model Loading");
                ImGui::BulletText("ImGui Docking UI");
                ImGui::BulletText("Modular Architecture");
                ImGui::Separator();
                if (ImGui::Button("Close")) {
                    m_ShowAbout = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

    void LoadModel(const std::string& path) {
        auto model = ClaudeEngine::ModelLoaderFactory::LoadModel(path);
        if (model) {
            auto entity = m_ActiveScene->CreateEntity("Imported Model");
            auto& meshRenderer = entity.AddComponent<ClaudeEngine::MeshRendererComponent>(model);
            meshRenderer.MaterialOverride = m_DefaultMaterial;
        }
    }
    
    void NewScene() {
        m_ActiveScene = ClaudeEngine::CreateRef<ClaudeEngine::Scene>("Untitled Scene");
        m_HierarchyPanel->SetContext(m_ActiveScene);
        m_ViewportPanel->SetContext(m_ActiveScene);
        m_CurrentScenePath = "";
        CreateDefaultScene();
    }
    
    void SaveScene() {
        if (m_CurrentScenePath.empty()) {
            SaveSceneAs();
        } else {
            ClaudeEngine::SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_CurrentScenePath);
        }
    }
    
    void SaveSceneAs() {
        // Simple file path input for now (TODO: use proper file dialog)
        m_CurrentScenePath = "assets/scenes/" + m_ActiveScene->GetName() + ".yaml";
        ClaudeEngine::SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(m_CurrentScenePath);
    }
    
    void OpenScene() {
        // TODO: File dialog
        // For now, try to load a hardcoded scene
        std::string filepath = "assets/scenes/TestScene.yaml";
        ClaudeEngine::SceneSerializer serializer(m_ActiveScene);
        if (serializer.Deserialize(filepath)) {
            m_HierarchyPanel->SetContext(m_ActiveScene);
            m_ViewportPanel->SetContext(m_ActiveScene);
            m_CurrentScenePath = filepath;
        }
    }
    
    void CreatePrimitive(const std::string& name, ClaudeEngine::PrimitiveType type) {
        auto entity = m_ActiveScene->CreateEntity(name);
        auto& meshRenderer = entity.AddComponent<ClaudeEngine::MeshRendererComponent>();
        
        std::shared_ptr<ClaudeEngine::Mesh> mesh;
        
        switch (type) {
            case ClaudeEngine::PrimitiveType::Cube:
                mesh = ClaudeEngine::MeshPrimitives::CreateCube(1.0f);
                break;
            case ClaudeEngine::PrimitiveType::Sphere:
                mesh = ClaudeEngine::MeshPrimitives::CreateSphere(1.0f, 32, 16);
                break;
            case ClaudeEngine::PrimitiveType::Plane:
                mesh = ClaudeEngine::MeshPrimitives::CreatePlane(10.0f, 10.0f, 1);
                break;
            case ClaudeEngine::PrimitiveType::Cylinder:
                mesh = ClaudeEngine::MeshPrimitives::CreateCylinder(0.5f, 2.0f, 32);
                break;
            case ClaudeEngine::PrimitiveType::Cone:
                mesh = ClaudeEngine::MeshPrimitives::CreateCone(1.0f, 2.0f, 32);
                break;
            case ClaudeEngine::PrimitiveType::Torus:
                mesh = ClaudeEngine::MeshPrimitives::CreateTorus(1.0f, 0.3f, 32, 16);
                break;
            case ClaudeEngine::PrimitiveType::Capsule:
                mesh = ClaudeEngine::MeshPrimitives::CreateCapsule(0.5f, 2.0f, 16, 8);
                break;
        }
        
        if (mesh) {
            // Create a simple model from mesh
            auto model = ClaudeEngine::CreateRef<ClaudeEngine::Model>();
            model->AddMesh(mesh);
            meshRenderer.ModelAsset = model;
            meshRenderer.MaterialOverride = m_DefaultMaterial;
        }
    }

private:
    // Scene
    ClaudeEngine::Ref<ClaudeEngine::Scene> m_ActiveScene;
    ClaudeEngine::Entity m_CameraEntity;
    
    // Camera (legacy for now)
    ClaudeEngine::Scope<ClaudeEngine::PerspectiveCamera> m_Camera;
    
    // Rendering
    ClaudeEngine::Ref<ClaudeEngine::Shader> m_Shader;
    ClaudeEngine::Ref<ClaudeEngine::Material> m_DefaultMaterial;
    ClaudeEngine::Ref<ClaudeEngine::MaterialLibrary> m_MaterialLibrary;
    ClaudeEngine::Ref<ClaudeEngine::Framebuffer> m_Framebuffer;
    
    // Panels
    ClaudeEngine::Scope<ClaudeEngine::HierarchyPanel> m_HierarchyPanel;
    ClaudeEngine::Scope<ClaudeEngine::ContentBrowserPanel> m_ContentBrowser;
    ClaudeEngine::Scope<ClaudeEngine::PropertiesPanel> m_PropertiesPanel;
    ClaudeEngine::Scope<ClaudeEngine::SettingsPanel> m_SettingsPanel;
    ClaudeEngine::Scope<ClaudeEngine::StatsPanel> m_StatsPanel;
    ClaudeEngine::Scope<ClaudeEngine::ViewportPanel> m_ViewportPanel;
    
    // Panel visibility
    bool m_ShowHierarchy = true;
    bool m_ShowContentBrowser = true;
    bool m_ShowProperties = true;
    bool m_ShowSettings = false;
    bool m_ShowStats = true;
    
    glm::vec3 m_ObjectColor = { 0.3f, 0.7f, 0.9f };
    float m_Rotation = 0.0f;
    bool m_ShowAbout = false;
    
    // Scene management
    std::string m_CurrentScenePath = "";
};

class EditorApplication : public ClaudeEngine::Application {
public:
    EditorApplication() : Application("Claude Engine - Editor") {
        // Apply professional dark theme
        ClaudeEngine::EditorTheme::SetProfessionalDarkTheme();
        
        m_EditorLayer = ClaudeEngine::CreateScope<EditorLayer>();
        
        // Try to load a default model (optional)
        // m_EditorLayer->LoadModel("assets/models/cube.obj");
    }

    ~EditorApplication() {}

protected:
    void OnUpdate(float deltaTime) override {
        m_EditorLayer->OnUpdate(deltaTime);
    }

    void OnRender() override {
        m_EditorLayer->OnRender();
    }

    void OnImGuiRender() override {
        m_EditorLayer->OnImGuiRender();
    }

    void OnEvent(ClaudeEngine::Event& e) {
        m_EditorLayer->OnEvent(e);
    }

private:
    ClaudeEngine::Scope<EditorLayer> m_EditorLayer;
};

ClaudeEngine::Application* ClaudeEngine::CreateApplication() {
    return new EditorApplication();
}

int main(int argc, char** argv) {
    try {
        CE_INFO("=== Starting Claude Engine Editor ===");
        auto app = ClaudeEngine::CreateApplication();
        CE_INFO("Application created, starting main loop...");
        app->Run();
        CE_INFO("Application finished, cleaning up...");
        delete app;
        CE_INFO("=== Claude Engine Editor closed successfully ===");
        return 0;
    } catch (const std::exception& e) {
        CE_CORE_ERROR("Fatal error: {0}", e.what());
        std::cerr << "\n\nFATAL ERROR: " << e.what() << std::endl;
        std::cerr << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    } catch (...) {
        CE_CORE_ERROR("Fatal error: Unknown exception");
        std::cerr << "\n\nFATAL ERROR: Unknown exception" << std::endl;
        std::cerr << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }
}
