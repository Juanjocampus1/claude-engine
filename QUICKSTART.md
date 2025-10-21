# Quick Start Guide - Claude Engine

Guía rápida para empezar a usar el motor Claude Engine.

## 📦 Instalación

### Paso 1: Clonar y Setup
```powershell
# Clonar repositorio
git clone <tu-repo-url> Claude-Engine
cd Claude-Engine

# Ejecutar script de setup (descarga dependencias)
.\setup.ps1
```

### Paso 2: Compilar
```powershell
# Build automático
.\build.ps1

# O manualmente:
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### Paso 3: Ejecutar
```powershell
.\build\bin\Release\ClaudeEditor.exe
```

## 🎨 Primer Proyecto

### Crear una Escena Básica

```cpp
#include <ClaudeEngine/ClaudeEngine.h>

class MyApp {
public:
    MyApp() {
        // 1. Crear escena
        m_Scene = ClaudeEngine::CreateRef<ClaudeEngine::Scene>("Mi Primera Escena");
        
        // 2. Crear cámara
        SetupCamera();
        
        // 3. Crear luz
        SetupLight();
        
        // 4. Cargar modelo
        LoadModel();
    }
    
private:
    void SetupCamera() {
        auto camera = m_Scene->CreateEntity("Main Camera");
        auto& camComp = camera.AddComponent<ClaudeEngine::CameraComponent>();
        camComp.Primary = true;
        camComp.FOV = 45.0f;
        camComp.NearClip = 0.1f;
        camComp.FarClip = 1000.0f;
        
        auto& transform = camera.GetComponent<ClaudeEngine::TransformComponent>();
        transform.Translation = {0.0f, 2.0f, 5.0f};
    }
    
    void SetupLight() {
        auto light = m_Scene->CreateEntity("Directional Light");
        auto& lightComp = light.AddComponent<ClaudeEngine::LightComponent>();
        lightComp.Type = ClaudeEngine::LightType::Directional;
        lightComp.Color = {1.0f, 1.0f, 1.0f};
        lightComp.Intensity = 1.0f;
        
        auto& transform = light.GetComponent<ClaudeEngine::TransformComponent>();
        transform.Rotation = glm::radians(glm::vec3(-45.0f, -30.0f, 0.0f));
    }
    
    void LoadModel() {
        // Cargar modelo con el factory
        auto model = ClaudeEngine::ModelLoaderFactory::LoadModel("assets/models/cube.obj");
        
        if (model) {
            auto entity = m_Scene->CreateEntity("Mi Cubo");
            auto& renderer = entity.AddComponent<ClaudeEngine::MeshRendererComponent>(model);
            renderer.CastShadows = true;
            renderer.ReceiveShadows = true;
            
            // Opcional: Material personalizado
            SetupMaterial(renderer);
        }
    }
    
    void SetupMaterial(ClaudeEngine::MeshRendererComponent& renderer) {
        // Cargar shader
        auto shader = ClaudeEngine::Shader::Create("assets/shaders/PBR_RayTracing.glsl");
        
        // Crear material
        auto material = ClaudeEngine::MaterialLibrary::CreateDefaultRayTracing();
        material->SetShader(shader);
        
        // Configurar propiedades PBR
        material->SetFloat("Material.Metallic", 0.5f);
        material->SetFloat("Material.Roughness", 0.5f);
        material->SetFloat("Material.IOR", 1.45f);
        material->SetVec3("Material.Albedo", {0.8f, 0.2f, 0.2f});
        
        renderer.MaterialOverride = material;
    }
    
    ClaudeEngine::Ref<ClaudeEngine::Scene> m_Scene;
};
```

## 🎮 Usar el Editor

### Paneles Principales

#### 1. Hierarchy (Jerarquía)
- **Crear Entidad**: Botón "Create Entity"
- **Crear Cámara**: Botón "Create Camera"
- **Crear Luz**: Botón "Create Light"
- **Seleccionar**: Click izquierdo en una entidad
- **Eliminar**: Click derecho > Delete

#### 2. Content Browser
- **Navegar**: Click en carpetas en el árbol izquierdo
- **Ver Assets**: Grid de assets en el panel derecho
- **Importar**: Drag & drop desde el explorador de archivos

#### 3. Properties (Inspector)
- **Transform**: Editar Position, Rotation, Scale
- **Components**: Añadir/editar componentes de la entidad seleccionada
- **Add Component**: Botón para añadir nuevos componentes

#### 4. Settings
- **Render**: VSync, MSAA, Resolution Scale
- **Physics**: Gravity, Fixed Timestep
- **Editor**: Camera Speed, Grid, Gizmos

#### 5. Stats
- Ver FPS, Frame Time, Draw Calls

## 📝 Ejemplos Comunes

### Cargar Diferentes Formatos de Modelo

```cpp
// FBX
auto fbxModel = ClaudeEngine::ModelLoaderFactory::LoadModel("model.fbx");
auto fbxEntity = scene->CreateEntity("FBX Model");
fbxEntity.AddComponent<ClaudeEngine::FBXModelComponent>("model.fbx", fbxModel);

// OBJ
auto objModel = ClaudeEngine::ModelLoaderFactory::LoadModel("model.obj");
auto objEntity = scene->CreateEntity("OBJ Model");
objEntity.AddComponent<ClaudeEngine::OBJModelComponent>("model.obj", objModel);

// GLTF
auto gltfModel = ClaudeEngine::ModelLoaderFactory::LoadModel("model.gltf");
auto gltfEntity = scene->CreateEntity("GLTF Model");
gltfEntity.AddComponent<ClaudeEngine::GLTFModelComponent>("model.gltf", gltfModel);
```

### Crear Diferentes Tipos de Luz

```cpp
// Luz Direccional (Sol)
auto dirLight = scene->CreateEntity("Sun");
auto& dirLightComp = dirLight.AddComponent<ClaudeEngine::LightComponent>();
dirLightComp.Type = ClaudeEngine::LightType::Directional;
dirLightComp.Color = {1.0f, 0.95f, 0.8f};
dirLightComp.Intensity = 1.0f;

// Luz Punto (Bombilla)
auto pointLight = scene->CreateEntity("Bulb");
auto& pointLightComp = pointLight.AddComponent<ClaudeEngine::LightComponent>();
pointLightComp.Type = ClaudeEngine::LightType::Point;
pointLightComp.Color = {1.0f, 0.8f, 0.6f};
pointLightComp.Intensity = 2.0f;
pointLightComp.Range = 10.0f;

// Luz Spot (Linterna)
auto spotLight = scene->CreateEntity("Spotlight");
auto& spotLightComp = spotLight.AddComponent<ClaudeEngine::LightComponent>();
spotLightComp.Type = ClaudeEngine::LightType::Spot;
spotLightComp.Color = {1.0f, 1.0f, 1.0f};
spotLightComp.Intensity = 5.0f;
spotLightComp.Range = 15.0f;
spotLightComp.InnerConeAngle = 15.0f;
spotLightComp.OuterConeAngle = 25.0f;
```

### Materiales Avanzados

```cpp
// Material Metálico
auto metalMat = ClaudeEngine::MaterialLibrary::CreateDefaultRayTracing();
metalMat->SetShader(shader);
metalMat->SetFloat("Material.Metallic", 1.0f);
metalMat->SetFloat("Material.Roughness", 0.2f);
metalMat->SetVec3("Material.Albedo", {0.8f, 0.8f, 0.8f});

// Material Vidrio
auto glassMat = ClaudeEngine::MaterialLibrary::CreateDefaultRayTracing();
glassMat->SetShader(shader);
glassMat->SetFloat("Material.Metallic", 0.0f);
glassMat->SetFloat("Material.Roughness", 0.0f);
glassMat->SetFloat("Material.IOR", 1.5f);
glassMat->SetFloat("Material.Transmission", 1.0f);

// Material Emisivo
auto emissiveMat = ClaudeEngine::MaterialLibrary::CreateDefaultRayTracing();
emissiveMat->SetShader(shader);
emissiveMat->SetVec3("Material.Emission", {1.0f, 0.5f, 0.0f});
emissiveMat->SetFloat("Material.EmissionStrength", 5.0f);
```

### Transformaciones

```cpp
auto entity = scene->CreateEntity("Rotating Cube");
auto& transform = entity.GetComponent<ClaudeEngine::TransformComponent>();

// Posición
transform.Translation = {0.0f, 1.0f, 0.0f};

// Rotación (en radianes)
transform.Rotation = glm::radians(glm::vec3(45.0f, 30.0f, 0.0f));

// Escala
transform.Scale = {2.0f, 1.0f, 1.0f};

// Obtener matriz de transformación
glm::mat4 transformMatrix = transform.GetTransform();
```

### Sistema de Componentes

```cpp
auto entity = scene->CreateEntity("Complex Object");

// Añadir múltiples componentes
entity.AddComponent<ClaudeEngine::MeshRendererComponent>();
entity.AddComponent<ClaudeEngine::RigidbodyComponent>();
entity.AddComponent<ClaudeEngine::ColliderComponent>();

// Verificar si tiene componente
if (entity.HasComponent<ClaudeEngine::RigidbodyComponent>()) {
    // Obtener y modificar
    auto& rb = entity.GetComponent<ClaudeEngine::RigidbodyComponent>();
    rb.Mass = 10.0f;
    rb.UseGravity = true;
    rb.Velocity = {0.0f, 5.0f, 0.0f};
}

// Eliminar componente
entity.RemoveComponent<ClaudeEngine::ColliderComponent>();
```

## 🎓 Conceptos Clave

### Entity Component System (ECS)
- **Entity**: Contenedor con UUID único
- **Component**: Datos puros sin lógica
- **System**: Lógica que procesa componentes (en Scene::OnUpdate)

### Materiales y Shaders
- Los shaders se cargan desde archivos `.glsl`
- Formato: `#type vertex` y `#type fragment`
- Los materiales almacenan propiedades y texturas
- Bind automático en el render loop

### Carga de Modelos
- Factory pattern detecta el formato automáticamente
- Soporta FBX, OBJ, GLTF, GLB y más
- Assimp procesa los datos internamente
- Genera Mesh con Vertex Buffers automáticamente

## 🐛 Troubleshooting

### CMake no encuentra Visual Studio
```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

### Error al cargar modelo
- Verifica que el path sea correcto
- Asegúrate que el formato esté soportado
- Revisa los logs en consola

### Shader no compila
- Verifica sintaxis GLSL
- Revisa que tenga `#type vertex` y `#type fragment`
- Consulta los logs de OpenGL

### ImGui no aparece
- Verifica que GLFW esté inicializado
- Asegúrate que ImGui::Render() se llame cada frame

## 📚 Recursos

- [OpenGL Documentation](https://docs.gl/)
- [GLSL Reference](https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language)
- [EnTT Documentation](https://github.com/skypjack/entt)
- [Assimp Documentation](https://assimp-docs.readthedocs.io/)
- [ImGui Documentation](https://github.com/ocornut/imgui)

## 🎯 Próximos Pasos

1. **Lee el README.md** para entender la arquitectura
2. **Explora el código** de ejemplo en `Editor/src/EditorApp.cpp`
3. **Crea tu primera escena** siguiendo los ejemplos
4. **Experimenta** con materiales y luces
5. **Importa modelos 3D** en diferentes formatos

¡Diviértete creando con Claude Engine! 🚀
