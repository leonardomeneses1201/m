cmake_minimum_required(VERSION 3.18)
project(CraftCPP CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Windows-specific: prefer static runtime (MSVC) ───────────────────────────
if(MSVC)
    # /W3 warnings, /MP parallel build, /utf-8 source encoding
    add_compile_options(/W3 /MP /utf-8)
    # Suppress "unsafe" CRT warnings (sprintf, etc.)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    # Static CRT to avoid vcruntime DLL dependency on end-user machines
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ── Dependencies ──────────────────────────────────────────────────────────────
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glfw3  REQUIRED)
find_package(glm    REQUIRED)

# ── Sources ───────────────────────────────────────────────────────────────────
add_executable(CraftCPP
    src/main.cpp
    src/Game.cpp
    src/Chunk.cpp
)

target_include_directories(CraftCPP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ── Link libraries ────────────────────────────────────────────────────────────
target_link_libraries(CraftCPP PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

# Linux needs dl
if(UNIX AND NOT APPLE)
    target_link_libraries(CraftCPP PRIVATE dl)
endif()

# ── Windows: link against additional system libs ──────────────────────────────
if(WIN32)
    target_link_libraries(CraftCPP PRIVATE gdi32 user32 shell32)
    # Copy GLFW and GLEW DLLs to output dir automatically
    add_custom_command(TARGET CraftCPP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CraftCPP>
            $<TARGET_FILE_DIR:CraftCPP>
        COMMAND_EXPAND_LISTS
    )
endif()

# ── Build type defaults ───────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# GCC/Clang optimizations
if(NOT MSVC)
    target_compile_options(CraftCPP PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native>
        $<$<CONFIG:Debug>:-g -O0>
    )
endif()

# MSVC optimizations
if(MSVC)
    target_compile_options(CraftCPP PRIVATE
        $<$<CONFIG:Release>:/O2 /Ob2 /DNDEBUG>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
endif()

message(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler   : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "OpenGL     : ${OPENGL_gl_LIBRARY}")
