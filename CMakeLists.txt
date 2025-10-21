cmake_minimum_required(VERSION 3.20)
project(ClaudeEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Dependencies directory
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Add dependencies
add_subdirectory(dependencies)

# Engine library
add_subdirectory(Engine)

# Editor executable
add_subdirectory(Editor)
