project(ClaudeEngine)

# Collect all source files
file(GLOB_RECURSE ENGINE_SOURCES 
    "src/*.cpp"
)

file(GLOB_RECURSE ENGINE_HEADERS 
    "include/*.h"
)

# Create engine library
add_library(${PROJECT_NAME} STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glfw
        glad
        glm
        assimp
        imgui
        stb_image
        entt
        imguizmo
        yaml-cpp
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC GL X11 pthread dl)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
endif()

# Set compile definitions
target_compile_definitions(${PROJECT_NAME} 
    PUBLIC
        $<$<CONFIG:Debug>:CE_DEBUG>
        $<$<CONFIG:Release>:CE_RELEASE>
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
