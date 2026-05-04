#include "Game.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ── Constructor / Destructor ──────────────────────────────────────────────────

Game::Game()  { init(); }

Game::~Game() {
    if (atlasTexture) glDeleteTextures(1, &atlasTexture);
    if (crosshairVAO) { glDeleteVertexArrays(1, &crosshairVAO); glDeleteBuffers(1, &crosshairVBO); }
    if (wireVAO)      { glDeleteVertexArrays(1, &wireVAO);      glDeleteBuffers(1, &wireVBO); }
    glfwDestroyWindow(window);
    glfwTerminate();
}

// ── Initialisation ────────────────────────────────────────────────────────────

void Game::init() {
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "CraftCPP", nullptr, nullptr);
    if (!window) throw std::runtime_error("glfwCreateWindow failed");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) throw std::runtime_error("glewInit failed");

    // Callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback           (window, cbKey);
    glfwSetMouseButtonCallback   (window, cbMouseBtn);
    glfwSetScrollCallback        (window, cbScroll);
    glfwSetFramebufferSizeCallback(window, cbResize);

    // Capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    chunkShader   = std::make_unique<Shader>(CHUNK_VERT,   CHUNK_FRAG);
    overlayShader = std::make_unique<Shader>(OVERLAY_VERT, OVERLAY_FRAG);
    wireShader    = std::make_unique<Shader>(WIRE_VERT,    WIRE_FRAG);

    // Resources
    generateAtlas();
    initCrosshair();
    initWireframe();

    // World + camera
    world = std::make_unique<World>(12345u);

    // Pre-load a ring so the player spawns on terrain
    for (int dx = -3; dx <= 3; dx++)
        for (int dz = -3; dz <= 3; dz++)
            world->loadChunk(dx, dz);

    int spawnY = world->findSpawnY(8, 8);
    camera = std::make_unique<Camera>(glm::vec3(8.5f, (float)spawnY, 8.5f));
}

// ── Main loop ─────────────────────────────────────────────────────────────────

void Game::run() {
    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt  = std::min(now - lastTime, 0.05f);   // cap at 50 ms
        lastTime  = now;

        // Mouse delta
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        if (firstMouse) { lastMouseX = mx; lastMouseY = my; firstMouse = false; }
        if (mouseCaptured)
            camera->processMouseMovement((float)(mx - lastMouseX),
                                         (float)(my - lastMouseY));
        lastMouseX = mx; lastMouseY = my;

        processInput(dt);
        update(dt);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ── Input ─────────────────────────────────────────────────────────────────────

void Game::processInput(float dt) {
    glm::vec3 move = { 0, 0, 0 };

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move += camera->front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move -= camera->front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move -= camera->right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move += camera->right;

    if (!flying) {
        move.y = 0.f;
        if (glm::length(move) > 0.f) move = glm::normalize(move);
        // Jump
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround) {
            velocity.y = 9.0f;
            onGround   = false;
        }
    } else {
        // Flying vertical
        if (glfwGetKey(window, GLFW_KEY_SPACE)      == GLFW_PRESS) move.y += 1.f;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) move.y -= 1.f;
        if (glm::length(move) > 0.f) move = glm::normalize(move);
    }

    float spd = camera->speed;
    if (flying) spd *= 4.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) spd *= 3.0f;

    camera->pos += move * spd * dt;
}

// ── Update ────────────────────────────────────────────────────────────────────

void Game::update(float dt) {
    if (!flying) applyPhysics(dt);

    int pcx = (int)std::floor(camera->pos.x / CHUNK_W);
    int pcz = (int)std::floor(camera->pos.z / CHUNK_D);
    world->update(pcx, pcz);

    // Raycast for block selection
    hasTarget = world->raycast(camera->pos, camera->front, 6.f,
                               targetBlock, targetNormal);
}

// ── Physics ───────────────────────────────────────────────────────────────────

void Game::applyPhysics(float dt) {
    const float GRAVITY = -28.0f;
    velocity.y += GRAVITY * dt;
    velocity.y  = std::max(velocity.y, -60.f);
    camera->pos.y += velocity.y * dt;

    // Foot position is 1.8 blocks below eye
    int bx = (int)std::floor(camera->pos.x);
    int by = (int)std::floor(camera->pos.y - 1.8f);
    int bz = (int)std::floor(camera->pos.z);

    if (isSolid(world->getBlock(bx, by, bz))) {
        camera->pos.y = by + 1.8f + 1.0f;
        velocity.y = 0.f;
        onGround = true;
    } else {
        onGround = false;
    }

    // Head collision
    int hy = (int)std::floor(camera->pos.y + 0.1f);
    if (isSolid(world->getBlock(bx, hy, bz)) && velocity.y > 0) {
        velocity.y = 0.f;
    }
}

// ── Render ────────────────────────────────────────────────────────────────────

void Game::render() {
    glClearColor(0.47f, 0.73f, 1.0f, 1.0f);   // sky blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int pcx = (int)std::floor(camera->pos.x / CHUNK_W);
    int pcz = (int)std::floor(camera->pos.z / CHUNK_D);
    float aspect = (float)width / (float)std::max(height, 1);

    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProjection(aspect);
    glm::mat4 mvp  = proj * view;

    // ── Chunk geometry ────────────────────────────────────────────────────────
    chunkShader->use();
    chunkShader->setMat4("uMVP", glm::value_ptr(mvp));
    chunkShader->setVec3("uCamPos", camera->pos.x, camera->pos.y, camera->pos.z);
    float fogNear = (world->renderDist - 2) * CHUNK_W * 0.7f;
    float fogFar  =  world->renderDist      * CHUNK_W * 0.85f;
    chunkShader->setFloat("uFogStart", fogNear);
    chunkShader->setFloat("uFogEnd",   fogFar);
    chunkShader->setVec3("uFogColor",  0.47f, 0.73f, 1.0f);
    chunkShader->setFloat("uDaylight", 1.0f);
    chunkShader->setInt("uAtlas", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    world->render(pcx, pcz);

    // ── Block selection wireframe ─────────────────────────────────────────────
    if (hasTarget) renderWireframe();

    // ── 2-D HUD ───────────────────────────────────────────────────────────────
    glDisable(GL_DEPTH_TEST);
    renderCrosshair();
    renderHUD();
    glEnable(GL_DEPTH_TEST);
}

// ── Procedural texture atlas ──────────────────────────────────────────────────
// 256×256 px, 16 tiles wide, each tile is 16×16 px.
// Tile indices match the UV offsets in Block.h.

static void noisyTile(std::vector<uint8_t>& px, int ATLAS,
                      int tx, int ty, uint8_t r, uint8_t g, uint8_t b, int var = 22)
{
    const int TILE = 16;
    srand((unsigned)(tx * 97 + ty * 31 + r + g * 3 + b * 7));
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            int v = (rand() % (var * 2 + 1)) - var;
            int idx = ((ty * TILE + dy) * ATLAS + (tx * TILE + dx)) * 4;
            px[idx+0] = (uint8_t)std::clamp(r + v, 0, 255);
            px[idx+1] = (uint8_t)std::clamp(g + v, 0, 255);
            px[idx+2] = (uint8_t)std::clamp(b + v, 0, 255);
            px[idx+3] = 255;
        }
    }
}

void Game::generateAtlas() {
    const int TILE = 16, ATLAS = 256;
    std::vector<uint8_t> pixels(ATLAS * ATLAS * 4, 0);

    auto setP = [&](int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
        int i = (y * ATLAS + x) * 4;
        pixels[i] = r; pixels[i+1] = g; pixels[i+2] = b; pixels[i+3] = a;
    };

    // Tile 0,0 — Grass top (green)
    noisyTile(pixels, ATLAS, 0, 0, 86, 160, 55, 18);

    // Tile 1,0 — Grass side (dirt lower half, green band top)
    noisyTile(pixels, ATLAS, 1, 0, 118, 84, 48, 16);
    for (int dy = 0; dy < 4; dy++)
        for (int dx = 0; dx < TILE; dx++) {
            int v = rand() % 14 - 7;
            setP(1*TILE+dx, 0*TILE+dy,
                 (uint8_t)std::clamp(76+v, 0, 255),
                 (uint8_t)std::clamp(148+v, 0, 255),
                 (uint8_t)std::clamp(48+v, 0, 255));
        }

    // Tile 2,0 — Dirt
    noisyTile(pixels, ATLAS, 2, 0, 118, 84, 48, 22);

    // Tile 3,0 — Stone
    noisyTile(pixels, ATLAS, 3, 0, 126, 126, 126, 28);
    // Add small crack lines
    srand(7);
    for (int k = 0; k < 6; k++) {
        int sx = 3*TILE + rand()%TILE, sy = 0*TILE + rand()%TILE;
        int len = 2 + rand()%4;
        for (int i = 0; i < len; i++)
            setP(sx + (rand()%3 - 1), sy + (rand()%3 - 1), 80, 80, 80);
    }

    // Tile 4,0 — Sand
    noisyTile(pixels, ATLAS, 4, 0, 210, 196, 136, 14);

    // Tile 5,0 — Wood top (annual rings)
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            float r = std::sqrt((dx-7.5f)*(dx-7.5f)+(dy-7.5f)*(dy-7.5f));
            int ring = ((int)(r * 1.2f)) % 2;
            int v = rand()%10-5;
            uint8_t base = ring ? 130 : 115;
            setP(5*TILE+dx, 0*TILE+dy,
                 std::clamp(base+v+20, 0, 255),
                 std::clamp(base+v, 0, 255),
                 std::clamp(base+v-30, 0, 255));
        }
    }

    // Tile 6,0 — Wood side (vertical grain)
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            float grain = std::sin(dx * 1.4f + dy * 0.1f) * 12.f;
            int v = rand()%8-4;
            setP(6*TILE+dx, 0*TILE+dy,
                 std::clamp(138+(int)grain+v, 0, 255),
                 std::clamp( 96+(int)(grain*0.7f)+v, 0, 255),
                 std::clamp( 44+(int)(grain*0.4f)+v, 0, 255));
        }
    }

    // Tile 7,0 — Leaves (semi-transparent)
    srand(42);
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            bool leaf = rand() % 5 != 0;
            if (leaf) {
                int v = rand()%20-10;
                setP(7*TILE+dx, 0*TILE+dy,
                     std::clamp(36+v, 0,255),
                     std::clamp(118+v, 0,255),
                     std::clamp(28+v, 0,255));
            } else {
                setP(7*TILE+dx, 0*TILE+dy, 0, 0, 0, 0);
            }
        }
    }

    // Tile 8,0 — Water
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            float wave = std::sin((dx + dy) * 0.65f) * 14.f;
            setP(8*TILE+dx, 0*TILE+dy,
                 18, (uint8_t)std::clamp(78+(int)wave, 0, 255),
                 (uint8_t)std::clamp(198+(int)(wave*0.4f), 0, 255), 210);
        }
    }

    // Tile 9,0 — Bedrock (dark speckled)
    srand(99);
    for (int dy = 0; dy < TILE; dy++) {
        for (int dx = 0; dx < TILE; dx++) {
            int v = rand() % 28;
            setP(9*TILE+dx, 0*TILE+dy,
                 std::clamp(22+v, 0, 255),
                 std::clamp(22+v, 0, 255),
                 std::clamp(26+v, 0, 255));
        }
    }

    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS, ATLAS, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// ── Crosshair ─────────────────────────────────────────────────────────────────

void Game::initCrosshair() {
    // Two short lines in NDC forming a + sign
    float s = 0.015f, t = 0.025f;
    float verts[] = {
        -t,  0,  t,  0,   // horizontal bar
         0, -s,  0,  s,   // vertical bar
    };
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Game::renderCrosshair() {
    overlayShader->use();
    overlayShader->setVec4("uColor", 1.f, 1.f, 1.f, 0.85f);
    glLineWidth(2.0f);
    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}

// ── Selection wireframe ───────────────────────────────────────────────────────

void Game::initWireframe() {
    // Unit cube edges (12 edges × 2 verts = 24 indices over 8 corners)
    float verts[8*3] = {
        0,0,0,  1,0,0,  1,1,0,  0,1,0,
        0,0,1,  1,0,1,  1,1,1,  0,1,1,
    };
    // clang-format off
    unsigned int idx[] = {
        0,1, 1,2, 2,3, 3,0,   // back face
        4,5, 5,6, 6,7, 7,4,   // front face
        0,4, 1,5, 2,6, 3,7,   // connecting edges
    };
    // clang-format on

    glGenVertexArrays(1, &wireVAO);
    glGenBuffers(1, &wireVBO);
    glBindVertexArray(wireVAO);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, wireVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Game::renderWireframe() {
    float pcx = (float)targetBlock.x - 0.002f;
    float pcy = (float)targetBlock.y - 0.002f;
    float pcz = (float)targetBlock.z - 0.002f;

    glm::mat4 model = glm::translate(glm::mat4(1.f), { pcx, pcy, pcz });
    model = glm::scale(model, { 1.004f, 1.004f, 1.004f });

    float aspect = (float)width / std::max(height, 1);
    glm::mat4 mvp = camera->getProjection(aspect) * camera->getView() * model;

    wireShader->use();
    wireShader->setMat4("uMVP", glm::value_ptr(mvp));

    glLineWidth(1.5f);
    glPolygonOffset(-1.f, -1.f);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glBindVertexArray(wireVAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_POLYGON_OFFSET_LINE);
}

// ── HUD title bar (block name + coords) ──────────────────────────────────────

void Game::renderHUD() {
    char title[192];
    const char* blockName = BLOCK_DATA[(int)selectedBlock].name;
    snprintf(title, sizeof(title),
             "CraftCPP  |  XYZ: %.1f / %.1f / %.1f  |  [%s]  |  F=fly(%s)  ESC=unlock",
             camera->pos.x, camera->pos.y, camera->pos.z,
             blockName, flying ? "on" : "off");
    glfwSetWindowTitle(window, title);
}

// ── Callbacks ─────────────────────────────────────────────────────────────────

void Game::cbKey(GLFWwindow* w, int key, int /*scan*/, int action, int /*mods*/) {
    auto* g = (Game*)glfwGetWindowUserPointer(w);
    if (action == GLFW_PRESS) {
        // Toggle mouse capture
        if (key == GLFW_KEY_ESCAPE) {
            g->mouseCaptured = !g->mouseCaptured;
            glfwSetInputMode(w, GLFW_CURSOR,
                g->mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
        // Toggle fly mode
        if (key == GLFW_KEY_F) {
            g->flying = !g->flying;
            g->velocity = { 0, 0, 0 };
        }
        // Quit
        if (key == GLFW_KEY_Q)
            glfwSetWindowShouldClose(w, GLFW_TRUE);

        // Block hotbar: keys 1-9
        for (int i = 0; i < (int)BlockType::COUNT - 1; i++) {
            if (key == GLFW_KEY_1 + i) {
                g->selectedBlock = (BlockType)(i + 1);
                break;
            }
        }
    }
}

void Game::cbMouseBtn(GLFWwindow* w, int btn, int action, int /*mods*/) {
    auto* g = (Game*)glfwGetWindowUserPointer(w);
    if (!g->mouseCaptured || !g->hasTarget || action != GLFW_PRESS) return;

    if (btn == GLFW_MOUSE_BUTTON_LEFT) {
        // Break block
        g->world->setBlock(g->targetBlock.x, g->targetBlock.y, g->targetBlock.z,
                           BlockType::AIR);
    } else if (btn == GLFW_MOUSE_BUTTON_RIGHT) {
        // Place selected block
        glm::ivec3 place = g->targetBlock + g->targetNormal;
        if (place.y > 0 && place.y < CHUNK_H)
            g->world->setBlock(place.x, place.y, place.z, g->selectedBlock);
    } else if (btn == GLFW_MOUSE_BUTTON_MIDDLE) {
        // Pick block type under cursor
        BlockType hit = g->world->getBlock(
            g->targetBlock.x, g->targetBlock.y, g->targetBlock.z);
        if (hit != BlockType::AIR) g->selectedBlock = hit;
    }
}

void Game::cbScroll(GLFWwindow* w, double /*dx*/, double dy) {
    auto* g = (Game*)glfwGetWindowUserPointer(w);
    int cur = (int)g->selectedBlock;
    int n   = (int)BlockType::COUNT - 1;  // number of non-air blocks
    cur = ((cur - 1 - (int)dy % n + n) % n) + 1;
    g->selectedBlock = (BlockType)cur;
}

void Game::cbResize(GLFWwindow* w, int width, int height) {
    auto* g = (Game*)glfwGetWindowUserPointer(w);
    g->width  = width;
    g->height = height;
    glViewport(0, 0, width, height);
}
