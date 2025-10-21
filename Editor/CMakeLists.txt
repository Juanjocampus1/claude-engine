project(ClaudeEditor)

# Collect source files
file(GLOB_RECURSE EDITOR_SOURCES 
    "src/*.cpp"
)

# Create executable
add_executable(${PROJECT_NAME}
    ${EDITOR_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link engine
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ClaudeEngine
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Keep console window open on Windows for debugging
if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Set startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
