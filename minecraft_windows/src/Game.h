#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/glm.hpp>
#include "Camera.h"
#include "World.h"
#include "Shader.h"
#include "blocks/Block.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    // ── Window ────────────────────────────────────────────────────────────────
    GLFWwindow* window = nullptr;
    int width = 1280, height = 720;

    // ── Core objects ──────────────────────────────────────────────────────────
    std::unique_ptr<Camera> camera;
    std::unique_ptr<World>  world;
    std::unique_ptr<Shader> chunkShader;
    std::unique_ptr<Shader> overlayShader;  // 2-D crosshair / HUD
    std::unique_ptr<Shader> wireShader;     // block selection outline

    // ── OpenGL handles ────────────────────────────────────────────────────────
    GLuint atlasTexture = 0;
    GLuint crosshairVAO = 0, crosshairVBO = 0;
    GLuint wireVAO      = 0, wireVBO      = 0;

    // ── Game state ────────────────────────────────────────────────────────────
    BlockType selectedBlock = BlockType::GRASS;

    bool   mouseCaptured = true;
    bool   firstMouse    = true;
    double lastMouseX    = 0, lastMouseY = 0;
    float  lastTime      = 0.f;

    // Physics
    glm::vec3 velocity  = { 0, 0, 0 };
    bool      onGround  = false;
    bool      flying    = true;

    // Raycast target
    bool       hasTarget  = false;
    glm::ivec3 targetBlock  = { 0, 0, 0 };
    glm::ivec3 targetNormal = { 0, 0, 0 };

    // ── Private methods ───────────────────────────────────────────────────────
    void init();
    void processInput(float dt);
    void update(float dt);
    void render();
    void applyPhysics(float dt);
    void generateAtlas();
    void initCrosshair();
    void initWireframe();
    void renderCrosshair();
    void renderWireframe();
    void renderHUD();

    // ── GLFW static callbacks ─────────────────────────────────────────────────
    static void cbKey      (GLFWwindow*, int, int, int, int);
    static void cbMouseBtn (GLFWwindow*, int, int, int);
    static void cbScroll   (GLFWwindow*, double, double);
    static void cbResize   (GLFWwindow*, int, int);
};
