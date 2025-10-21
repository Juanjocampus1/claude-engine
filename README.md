# Claude Engine

Motor gráfico moderno construido con OpenGL y C++17, con arquitectura modular preparada para Vulkan.

## 🎯 Características Principales

### Arquitectura Multi-Capa
- **Core**: Sistema de logging, aplicación base, eventos
- **Platform**: Abstracción de ventana (GLFW), implementaciones específicas
- **Renderer**: API de renderizado abstracta (OpenGL actual, Vulkan futuro)
- **Scene**: Sistema Entity-Component-System con EnTT
- **Editor**: Editor profesional con ImGui y docking

### Sistema de Renderizado
- ✅ Abstracción RenderAPI preparado para OpenGL/Vulkan
- ✅ Buffers (Vertex, Index) con layout flexible
- ✅ Vertex Arrays para gestión de geometría
- ✅ **Shaders con carga desde archivos .glsl**
- ✅ Texturas 2D con soporte stb_image
- ✅ Cámara perspectiva con transformaciones
- ✅ **Sistema de materiales con enfoque en ray tracing**

### Sistema de Materiales
- **Workflows**: PBR Metallic-Roughness, PBR Specular-Glossiness, Unlit, Ray Tracing
- **Propiedades Ray Tracing**: IOR, Transmisión, Clearcoat, Sheen, Anisotropía, Subsurface Scattering
- **Mapas de textura**: Albedo, Normal, Metallic-Roughness, AO, Emission

### Carga de Modelos 3D
- ✅ **Assimp** para importar múltiples formatos (40+ formatos soportados)
- ✅ Abstracción con factory pattern
- ✅ Loaders específicos: **FBX**, **OBJ**, **GLTF/GLB**
- ✅ Componentes dedicados para cada formato

### Entity Component System (ECS)
Basado en **EnTT**, sistema ultrarrápido con componentes:
- Core: ID, Tag, Transform
- Rendering: MeshRenderer, FBXModel, OBJModel, GLTFModel
- Lighting: Light (Directional/Point/Spot/Area)
- Camera, Rigidbody, Collider, Script

### Editor Profesional
- **Hierarchy Panel** (Unity-style): Gestión de entidades con árbol
- **Content Browser** (Unity-style): Navegador de assets con drag & drop
- **Properties Panel**: Inspector de componentes con edición en tiempo real
- **Settings Panel** (Blender-style): Configuración con tabs (Render/Physics/Editor/Project)
- **Stats Panel**: FPS, frame time, draw calls, vertices

## Architecture

```
ClaudeEngine/
├── Core/           # Application, logging, assertions
├── Events/         # Event system
├── Platform/       # Window, input, OpenGL implementations
├── Renderer/       # Rendering abstraction layer
│   ├── RenderAPI   # Abstract rendering interface
│   ├── Shader      # Shader management
│   ├── Buffer      # Vertex/Index buffers
│   ├── Texture     # Texture loading
│   ├── Camera      # Camera systems
│   ├── Mesh        # Mesh representation
│   └── Model       # Model loading with Assimp
└── Editor/         # ImGui-based editor application
```

## Requirements

- CMake 3.20+
- C++17 compiler (MSVC 2022, GCC, Clang)
- Git
- OpenGL 4.6 capable GPU

## Quick Start

### 1. Clone and Setup

```powershell
# Clone the repository
git clone <your-repo-url> Claude-Engine
cd Claude-Engine

# Run setup script (downloads all dependencies)
.\setup.ps1
```

### 2. Build

```powershell
# Build the project
.\build.ps1

# Or manually:
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 3. Run

```powershell
.\build\bin\Release\ClaudeEditor.exe
```

## 🛠️ Tecnologías

### Dependencias (Git Submodules)
- **GLFW**: Gestión de ventanas e input
- **GLM**: Matemáticas para gráficos
- **Assimp**: Importación de modelos 3D (40+ formatos)
- **ImGui** (docking branch): UI del editor con sistema de docking
- **EnTT**: Entity Component System header-only ultra-rápido
- **GLAD**: Cargador de funciones OpenGL 4.6 Core
- **stb_image**: Carga de imágenes (single-header)

## Editor Controls

- **WASD** - Move camera horizontally
- **Q/E** - Move camera up/down
- **Mouse** - Look around (when implemented)

## Project Structure

```
Claude-Engine/
├── dependencies/       # External libraries (git submodules)
├── Engine/            # Engine library
│   ├── include/       # Public headers
│   └── src/          # Implementation files
├── Editor/           # Editor application
│   └── src/          # Editor source
├── assets/           # Assets (models, textures, shaders)
├── build/            # CMake build directory
├── setup.ps1         # Setup script
├── build.ps1         # Build script
└── CMakeLists.txt    # Root CMake configuration
```

## 🎮 Uso del Engine

### Crear una Escena
```cpp
// Crear escena
auto scene = CreateRef<Scene>("Mi Escena");

// Crear entidad con modelo
auto entity = scene->CreateEntity("Mi Objeto");

// Cargar modelo con factory pattern
auto model = ModelLoaderFactory::LoadModel("path/to/model.fbx");
auto& renderer = entity.AddComponent<MeshRendererComponent>(model);

// Crear material PBR con ray tracing
auto material = MaterialLibrary::CreateDefaultRayTracing();
material->SetShader(shader);
material->SetFloat("Material.Metallic", 0.8f);
material->SetFloat("Material.Roughness", 0.2f);
material->SetFloat("Material.IOR", 1.45f);
renderer.MaterialOverride = material;
```

### Usar el ECS
```cpp
// Añadir componentes
auto& transform = entity.GetComponent<TransformComponent>();
transform.Translation = {0.0f, 1.0f, 0.0f};
transform.Rotation = glm::radians(glm::vec3(45.0f, 0.0f, 0.0f));

// Crear luz
auto light = scene->CreateEntity("Directional Light");
auto& lightComp = light.AddComponent<LightComponent>();
lightComp.Type = LightType::Directional;
lightComp.Intensity = 1.0f;
```

### Cargar Shader desde Archivo
```cpp
// Formato con #type vertex y #type fragment
auto shader = Shader::Create("assets/shaders/PBR_RayTracing.glsl");
```

## 🔮 Roadmap Futuro

### Corto Plazo
- [ ] Framebuffers para render to texture
- [ ] Viewport interactivo con manipulación de escena
- [ ] Gizmos para transformación 3D
- [ ] Serialización de escenas (JSON/YAML)

### Medio Plazo
- [ ] Sistema de física completo
- [ ] Sistema de scripting (C# o Lua)
- [ ] Post-processing effects
- [ ] Shadow mapping
- [ ] PBR con IBL (Image-Based Lighting)

### Largo Plazo
- [ ] **Backend de Vulkan**
- [ ] **Ray tracing en hardware (RTX)**
- [ ] Editor multiplataforma
- [ ] Asset pipeline con hot-reload

## Adding New Rendering API

Thanks to the modular architecture, adding a new rendering API is straightforward:

1. Create `Platform/Vulkan/` directory
2. Implement `VulkanRenderAPI`, `VulkanBuffer`, `VulkanShader`, etc.
3. Update `RenderAPI::Create()` factory method
4. Switch API in `RenderAPI::s_API`

## Contributing

This is a learning/portfolio project, but contributions are welcome!

## License

MIT License - Feel free to use this code for learning or your own projects.

## Credits

Built by following modern C++ and engine architecture patterns.
Inspired by Hazel Engine and other open-source engines.
