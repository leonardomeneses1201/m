#pragma once
#include "Animal.h"
#include <cmath>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
//  OSO POLAR  —  Ataca si hay cachorros cerca. Bioma: hielo y nieves.
// ─────────────────────────────────────────────────────────────────────────────
class PolarBear : public Animal {
public:
    bool  hasCub         = false;
    float attackCooldown = 0.f;

    PolarBear(uint32_t id, glm::vec3 p, bool cub = false)
        : Animal(id, "Oso Polar"), hasCub(cub)
    {
        pos = p; maxHealth = health = 30;
        bodyW = 1.3f; bodyH = 1.4f;
        walkSpeed = 2.5f; fleeSpeed = 6.0f;
        drops = { {"Bacalao Crudo", 0, 2}, {"Salmón Crudo", 0, 2} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        attackCooldown -= dt;
        float dist = glm::length(pos - pp);
        if (hasCub && dist < 12.f && state != AnimalState::FLEEING) {
            state = AnimalState::FLEEING; stateTimer = 6.f;
        }
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.96f, 0.96f, 0.98f}; }
    glm::vec3 legColor()  const override { return {0.90f, 0.90f, 0.92f}; }
    const char* soundAmbient() const override { return "polar_bear_ambient"; }
    const char* soundHurt()    const override { return "polar_bear_hurt";    }
    const char* soundDeath()   const override { return "polar_bear_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  PANDA  —  7 personalidades. Estornuda de bebé. Muy raro en junglas.
// ─────────────────────────────────────────────────────────────────────────────
class Panda : public Animal {
public:
    enum class Personality : uint8_t {
        NORMAL, LAZY, WORRIED, PLAYFUL, AGGRESSIVE, WEAK, BROWN
    } personality;
    float sneezeCooldown = 0.f;
    bool  angry = false;

    Panda(uint32_t id, glm::vec3 p) : Animal(id, "Panda") {
        pos = p; maxHealth = health = 20;
        bodyW = 1.3f; bodyH = 1.25f;
        walkSpeed = 1.8f; fleeSpeed = 4.5f;
        drops = { {"Bambú", 1, 2} };
        int r = std::rand() % 100;
        personality = r < 40 ? Personality::NORMAL   :
                      r < 58 ? Personality::LAZY      :
                      r < 74 ? Personality::WORRIED   :
                      r < 86 ? Personality::PLAYFUL   :
                      r < 94 ? Personality::WEAK      :
                      r < 98 ? Personality::BROWN     : Personality::AGGRESSIVE;
        sneezeCooldown = 10.f + (float)(std::rand() % 10);
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if ((sneezeCooldown -= dt) <= 0.f)
            sneezeCooldown = 10.f + (float)(std::rand() % 10);
        if (personality == Personality::LAZY)    walkSpeed = 1.0f;
        if (personality == Personality::WORRIED) fleeSpeed = 7.0f;
        if (angry) { state = AnimalState::FLEEING; stateTimer = 5.f; }
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        return (personality == Personality::BROWN)
             ? glm::vec3{0.55f,0.38f,0.22f} : glm::vec3{0.96f,0.96f,0.96f};
    }
    glm::vec3 legColor() const override { return {0.08f, 0.08f, 0.08f}; }
    const char* soundAmbient() const override { return "panda_ambient"; }
    const char* soundHurt()    const override { return "panda_hurt";    }
    const char* soundDeath()   const override { return "panda_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CABRA  —  Embiste al jugador. Salta muy alto. 2% son "screaming goats".
// ─────────────────────────────────────────────────────────────────────────────
class Goat : public Animal {
public:
    bool  screaming  = false;
    float ramCooldown = 0.f, ramChargeT = 0.f;
    bool  charging   = false;

    Goat(uint32_t id, glm::vec3 p) : Animal(id, "Cabra") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.9f; bodyH = 1.3f;
        walkSpeed = 3.0f; fleeSpeed = 6.0f;
        drops = { {"Leche", 0, 1} };
        screaming = (std::rand() % 100 < 2);
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        ramCooldown -= dt;
        float dist = glm::length(pos - pp);
        if (!charging && ramCooldown <= 0.f && dist < 16.f && dist > 4.f) {
            glm::vec3 dir = glm::normalize(pp - pos); dir.y = 0.f;
            yaw = glm::degrees(std::atan2(dir.z, dir.x));
            charging = true; ramChargeT = 0.5f; state = AnimalState::IDLE;
        }
        if (charging && (ramChargeT -= dt) <= 0.f) {
            charging = false;
            ramCooldown = 30.f + (float)(std::rand() % 30);
            state = AnimalState::FLEEING; stateTimer = 3.0f;
        }
        if (onGround && (float)(std::rand() % 1000) < dt * 10.f) velocity.y = 8.0f;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.94f, 0.94f, 0.90f}; }
    glm::vec3 legColor()  const override { return {0.85f, 0.85f, 0.80f}; }
    const char* soundAmbient() const override { return screaming ? "goat_scream" : "goat_ambient"; }
    const char* soundHurt()    const override { return "goat_hurt";  }
    const char* soundDeath()   const override { return "goat_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  ZORRO  —  Nocturno. Ataca gallinas. Huye del jugador. Roba items.
// ─────────────────────────────────────────────────────────────────────────────
class Fox : public Animal {
public:
    enum class FoxColor : uint8_t { RED, ARCTIC } color;
    bool  sleeping   = false;
    float sleepTimer = 0.f;

    Fox(uint32_t id, glm::vec3 p) : Animal(id, "Zorro") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.6f; bodyH = 0.7f;
        walkSpeed = 3.0f; fleeSpeed = 7.0f;
        color = (std::rand() % 5 == 0) ? FoxColor::ARCTIC : FoxColor::RED;
        drops = { {"Experiencia", 1, 3} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if ((sleepTimer -= dt) <= 0.f) {
            sleeping = !sleeping;
            sleepTimer = sleeping ? 8.f : 15.f;
        }
        if (sleeping) return true;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        return (color == FoxColor::RED)
             ? glm::vec3{0.88f,0.42f,0.08f} : glm::vec3{0.96f,0.96f,0.94f};
    }
    glm::vec3 legColor() const override { return {0.10f, 0.08f, 0.06f}; }
    const char* soundAmbient() const override { return "fox_ambient"; }
    const char* soundHurt()    const override { return "fox_hurt";    }
    const char* soundDeath()   const override { return "fox_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  ARMADILLO  —  Se enrosca si el jugador se acerca corriendo.
//               Drops: Escamas (para armadura de perro). Sabanas/Badlands.
// ─────────────────────────────────────────────────────────────────────────────
class Armadillo : public Animal {
public:
    bool  curled      = false;
    float uncurlTimer = 0.f;

    Armadillo(uint32_t id, glm::vec3 p) : Animal(id, "Armadillo") {
        pos = p; maxHealth = health = 12;
        bodyW = 0.7f; bodyH = 0.65f;
        walkSpeed = 1.5f; fleeSpeed = 3.5f;
        drops = { {"Escamas de Armadillo", 0, 2} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if (uncurlTimer > 0.f && (uncurlTimer -= dt) <= 0.f) curled = false;
        float dist = glm::length(pos - pp);
        if (!curled && dist < 5.f) {
            curled = true; uncurlTimer = 4.0f; state = AnimalState::IDLE;
        }
        if (curled) return true;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.65f, 0.48f, 0.28f}; }
    glm::vec3 legColor()  const override { return {0.55f, 0.40f, 0.22f}; }
    const char* soundAmbient() const override { return "armadillo_ambient"; }
    const char* soundHurt()    const override { return "armadillo_hurt";    }
    const char* soundDeath()   const override { return "armadillo_death";   }
};
