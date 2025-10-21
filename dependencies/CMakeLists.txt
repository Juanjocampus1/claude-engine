# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLAD
add_library(glad STATIC 
    glad/src/glad.c
)
target_include_directories(glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# GLM
add_subdirectory(glm)

# Assimp
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(assimp)

# ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw glad)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# EnTT (header-only)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/single_include)

# ImGuizmo
add_library(imguizmo STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImCurveEdit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGradient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImSequencer.cpp
)
target_include_directories(imguizmo PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo
)
target_link_libraries(imguizmo PUBLIC imgui)

# yaml-cpp
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(yaml-cpp)
