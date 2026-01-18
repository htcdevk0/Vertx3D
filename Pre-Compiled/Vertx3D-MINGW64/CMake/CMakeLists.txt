# Made by: htcdevk0

# ===================================================
# Vertx3D CMake Configuration - User Project Setup
# ===================================================
cmake_minimum_required(VERSION 3.15)
project(YourProject LANGUAGES C CXX)

# --------------------------
# C++ Standard
# --------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------
# Windows DLL definition
# --------------------------
if(WIN32)
    add_definitions(-DVERTX3D_BUILD_DLL)
endif()

# --------------------------
# Include Directories
# --------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/include/vx_gui
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/include/vx_input

    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb

    # FreeType (adjust path if needed)
    C:/msys64/mingw64/include/freetype2
)

# --------------------------
# GLFW Subdirectory
# --------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)

# --------------------------
# Vertx3D Library Sources
# --------------------------
set(VERTX3D_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/core/vx_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/core/vx_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/core/vx_model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/core/vx_camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/core/vx_sound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/math/vx_physics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/vx_gui/vx_label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/vx_gui/vx_frame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Vertx3D/src/input/vx_mouse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

# --------------------------
# Create Vertx3D Shared Library
# --------------------------
add_library(vertx3d SHARED ${VERTX3D_SOURCES})

# --------------------------
# Linker Flags / Dependencies
# --------------------------
target_link_libraries(vertx3d
    glfw
    opengl32
    gdi32
    winmm
    C:/msys64/mingw64/lib/libfreetype.dll.a
)

# --------------------------
# User Executable (main.cpp in src/)
# --------------------------
# Build the user's main.cpp as the project executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link Vertx3D library
target_link_libraries(${PROJECT_NAME} vertx3d)

# --------------------------
# Post-Build: Copy DLL
# --------------------------
# Automatically copy the Vertx3D DLL next to the executable (Windows only)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:vertx3d>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/
    )
endif()

# ===================================================
# Notes:
# - User places their main.cpp (or other source files) in src/
# - Vertx3D library is in Vertx3D/ with headers in include/ and binaries in bin/
# - Build directory can be separate (build/)
# - No need to edit CMake manually; ready for distribution
# ===================================================
