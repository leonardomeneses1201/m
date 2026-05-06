#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Animal.h  –  Clase base para todas las criaturas pasivas y neutrales
// ─────────────────────────────────────────────────────────────────────────────
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include "../World.h"

// ── Estado de IA ──────────────────────────────────────────────────────────────
enum class AnimalState {
    IDLE,       // parado, mirando alrededor
    WALKING,    // caminando sin destino
    FLEEING,    // huyendo del jugador
    FOLLOWING,  // siguiendo al jugador (domesticado)
    EATING,     // comiendo hierba / flores
    DEAD,
};

// ── Drop al morir ─────────────────────────────────────────────────────────────
struct Drop {
    const char* item;
    int minQty;
    int maxQty;
};

// ── Clase base ────────────────────────────────────────────────────────────────
class Animal {
public:
    uint32_t    id;
    std::string name;
    glm::vec3   pos      = {0,0,0};
    glm::vec3   velocity = {0,0,0};
    float       yaw      = 0.f;     // dirección en grados

    int   health    = 10;
    int   maxHealth = 10;
    bool  alive     = true;
    bool  onGround  = false;

    // Dimensiones del bounding-box
    float bodyW = 1.0f;
    float bodyH = 1.0f;

    float walkSpeed  = 2.5f;
    float fleeSpeed  = 5.5f;

    AnimalState state      = AnimalState::IDLE;
    float       stateTimer = 0.f;   // tiempo restante en el estado actual

    std::vector<Drop> drops;

    Animal(uint32_t id, const std::string& name) : id(id), name(name) {}
    virtual ~Animal() = default;

    // ── Actualización por frame ───────────────────────────────────────────────
    virtual bool update(float dt, const glm::vec3& playerPos, World& world) {
        if (!alive) return false;
        stateTimer -= dt;

        // Máquina de estados básica
        if (stateTimer <= 0.f) {
            int r = std::rand() % 4;
            state      = (r == 0) ? AnimalState::IDLE : AnimalState::WALKING;
            stateTimer = 3.f + (float)(std::rand() % 5);
            if (state == AnimalState::WALKING)
                yaw = (float)(std::rand() % 360);
        }

        // Huir si el jugador está muy cerca
        float dist = glm::length(pos - playerPos);
        if (dist < 6.f && state != AnimalState::FOLLOWING) {
            state      = AnimalState::FLEEING;
            stateTimer = 4.f;
        }

        // Movimiento horizontal
        float spd = (state == AnimalState::FLEEING)   ? fleeSpeed  :
                    (state == AnimalState::FOLLOWING)  ? walkSpeed  :
                    (state == AnimalState::WALKING)    ? walkSpeed  : 0.f;

        if (spd > 0.f) {
            float rad = glm::radians(yaw);
            glm::vec3 dir = { std::cos(rad), 0.f, std::sin(rad) };
            if (state == AnimalState::FLEEING) {
                glm::vec3 away = glm::normalize(pos - playerPos);
                away.y = 0.f;
                dir = glm::length(away) > 0.01f ? glm::normalize(away) : dir;
            } else if (state == AnimalState::FOLLOWING) {
                glm::vec3 to = glm::normalize(playerPos - pos);
                to.y = 0.f;
                dir = glm::length(to) > 0.01f ? glm::normalize(to) : dir;
                if (dist < 2.5f) spd = 0.f;
            }
            pos += dir * spd * dt;
        }

        // Gravedad básica
        if (!onGround) {
            velocity.y -= 18.f * dt;
            pos.y      += velocity.y * dt;
        }

        // Suelo básico (World provee getBlock)
        int bx = (int)std::floor(pos.x);
        int by = (int)std::floor(pos.y - 0.05f);
        int bz = (int)std::floor(pos.z);
        if (isSolid(world.getBlock(bx, by, bz))) {
            pos.y      = (float)by + 1.0f;
            velocity.y = 0.f;
            onGround   = true;
        } else {
            onGround = false;
        }
        return true;
    }

    // ── Daño ─────────────────────────────────────────────────────────────────
    virtual void damage(int amount) {
        if (!alive) return;
        health -= amount;
        if (health <= 0) { health = 0; alive = false; state = AnimalState::DEAD; }
        // Huir al recibir daño
        else { state = AnimalState::FLEEING; stateTimer = 5.f; }
    }

    // ── Colores para el renderer ──────────────────────────────────────────────
    virtual glm::vec3 bodyColor() const { return {0.8f, 0.8f, 0.8f}; }
    virtual glm::vec3 headColor() const { return bodyColor(); }
    virtual glm::vec3 legColor()  const { return bodyColor() * 0.8f; }

    // ── Sonidos (nombres de eventos, el motor los busca en el atlas de audio) ─
    virtual const char* soundAmbient() const { return "generic_ambient"; }
    virtual const char* soundHurt()    const { return "generic_hurt";    }
    virtual const char* soundDeath()   const { return "generic_death";   }
};
