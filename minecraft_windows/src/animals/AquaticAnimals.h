#pragma once
#include "Animal.h"
#include <cmath>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
//  Base acuática: cancela gravedad en agua, pierde vida fuera de ella
// ─────────────────────────────────────────────────────────────────────────────
class AquaticAnimal : public Animal {
protected:
    bool  inWater   = true;
    float swimOscil = 0.f;
public:
    AquaticAnimal(uint32_t id, const std::string& name) : Animal(id, name) {}

    bool update(float dt, const glm::vec3& pp, World& w) override {
        BlockType bt = w.getBlock((int)pos.x, (int)pos.y, (int)pos.z);
        inWater = (bt == BlockType::WATER);
        if (inWater) {
            swimOscil += dt * 1.5f;
            velocity.y = std::sin(swimOscil) * 0.6f;
            onGround   = false;
        }
        if (!inWater && state != AnimalState::DEAD) {
            health -= (int)(dt * 1.5f + 0.5f);
            if (health <= 0) { health = 0; state = AnimalState::DEAD; alive = false; }
        }
        return Animal::update(dt, pp, w);
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  DELFÍN  —  Da efecto "Gracia del Delfín" al jugador cercano.
// ─────────────────────────────────────────────────────────────────────────────
class Dolphin : public AquaticAnimal {
public:
    float graceCooldown = 0.f;
    Dolphin(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Delfín") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.9f; bodyH = 0.6f;
        walkSpeed = 5.0f; fleeSpeed = 7.5f;
        drops = { {"Bacalao Crudo", 0, 1} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if ((graceCooldown -= dt) <= 0.f && glm::length(pos - pp) < 9.f && inWater)
            graceCooldown = 4.f;
        return AquaticAnimal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.25f, 0.40f, 0.75f}; }
    glm::vec3 legColor()  const override { return {0.25f, 0.40f, 0.75f}; }
    const char* soundAmbient() const override { return "dolphin_ambient"; }
    const char* soundHurt()    const override { return "dolphin_hurt";    }
    const char* soundDeath()   const override { return "dolphin_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  TORTUGA  —  Drops: Escamas (para casco tortuga). Pone huevos en la playa.
// ─────────────────────────────────────────────────────────────────────────────
class Turtle : public AquaticAnimal {
public:
    bool hasEggs = false;
    Turtle(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Tortuga") {
        pos = p; maxHealth = health = 30;
        bodyW = 1.2f; bodyH = 0.4f;
        walkSpeed = 0.8f; fleeSpeed = 1.5f;
        drops = { {"Escamas de Tortuga", 0, 1} };
    }
    glm::vec3 bodyColor() const override { return {0.20f, 0.55f, 0.20f}; }
    glm::vec3 legColor()  const override { return {0.25f, 0.60f, 0.25f}; }
    const char* soundAmbient() const override { return "turtle_ambient"; }
    const char* soundHurt()    const override { return "turtle_hurt";    }
    const char* soundDeath()   const override { return "turtle_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CALAMAR BRILLANTE  —  Drops: Tinta brillante. Vive en cuevas submarinas.
// ─────────────────────────────────────────────────────────────────────────────
class GlowSquid : public AquaticAnimal {
public:
    float glowPulse = 0.f;
    GlowSquid(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Calamar Brillante") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.8f; bodyH = 0.8f;
        walkSpeed = 2.0f; fleeSpeed = 4.0f;
        drops = { {"Saco de Tinta Brillante", 1, 3} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        glowPulse += dt * 2.0f;
        return AquaticAnimal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        float g = 0.65f + 0.35f * std::sin(glowPulse);
        return {0.05f, g * 0.85f, g};
    }
    glm::vec3 legColor() const override { return bodyColor(); }
    const char* soundAmbient() const override { return "squid_ambient"; }
    const char* soundHurt()    const override { return "squid_hurt";    }
    const char* soundDeath()   const override { return "squid_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  PECES  —  Bacalao, Salmón, Tropical, Pez Globo
// ─────────────────────────────────────────────────────────────────────────────
class Fish : public AquaticAnimal {
public:
    enum class Type : uint8_t { COD, SALMON, TROPICAL, PUFFERFISH } type;
    Fish(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Pez") {
        pos = p; maxHealth = health = 3;
        bodyW = 0.5f; bodyH = 0.4f;
        walkSpeed = 3.0f; fleeSpeed = 6.0f;
        type = (Type)(std::rand() % 4);
        switch (type) {
            case Type::COD:        name = "Bacalao";      drops = {{"Bacalao Crudo",  1,1}}; break;
            case Type::SALMON:     name = "Salmón";       drops = {{"Salmón Crudo",   1,1}}; break;
            case Type::TROPICAL:   name = "Pez Tropical"; drops = {{"Pez Tropical",   1,1}}; break;
            case Type::PUFFERFISH: name = "Pez Globo";    drops = {{"Pez Globo",      1,1}}; break;
        }
    }
    glm::vec3 bodyColor() const override {
        switch (type) {
            case Type::COD:        return {0.75f, 0.60f, 0.35f};
            case Type::SALMON:     return {0.80f, 0.35f, 0.25f};
            case Type::TROPICAL:   return {0.95f, 0.55f, 0.10f};
            case Type::PUFFERFISH: return {0.90f, 0.75f, 0.10f};
        } return {0.70f, 0.55f, 0.30f};
    }
    glm::vec3 legColor() const override { return bodyColor(); }
    const char* soundAmbient() const override { return "fish_ambient"; }
    const char* soundHurt()    const override { return "fish_hurt";    }
    const char* soundDeath()   const override { return "fish_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  AXOLOTL  —  5 colores. Cyan rarísimo: 1 entre 1200. Ayuda en combate.
// ─────────────────────────────────────────────────────────────────────────────
class Axolotl : public AquaticAnimal {
public:
    enum class Color : uint8_t { PINK, BROWN, GOLD, CYAN, BLUE } color;
    bool  playingDead  = false;
    float playDeadTimer = 0.f;

    Axolotl(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Axolotl") {
        pos = p; maxHealth = health = 14;
        bodyW = 0.75f; bodyH = 0.42f;
        walkSpeed = 1.8f; fleeSpeed = 4.0f;
        drops = {};
        int r = std::rand() % 1200;
        color = r < 1   ? Color::BLUE  :
                r < 120 ? Color::CYAN  :
                r < 360 ? Color::GOLD  :
                r < 720 ? Color::BROWN : Color::PINK;
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if (playingDead && (playDeadTimer -= dt) <= 0.f) playingDead = false;
        return AquaticAnimal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        switch (color) {
            case Color::PINK:  return {0.98f, 0.65f, 0.72f};
            case Color::BROWN: return {0.55f, 0.38f, 0.22f};
            case Color::GOLD:  return {0.95f, 0.78f, 0.20f};
            case Color::CYAN:  return {0.20f, 0.85f, 0.85f};
            case Color::BLUE:  return {0.10f, 0.30f, 0.90f};
        } return {0.98f, 0.65f, 0.72f};
    }
    glm::vec3 legColor() const override { return bodyColor() * 0.85f; }
    const char* soundAmbient() const override { return "axolotl_ambient"; }
    const char* soundHurt()    const override { return "axolotl_hurt";    }
    const char* soundDeath()   const override { return "axolotl_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  RANA  —  3 colores según bioma. Come magma cube → produce Froglight.
// ─────────────────────────────────────────────────────────────────────────────
class Frog : public Animal {
public:
    enum class Variant : uint8_t { TEMPERATE, COLD, WARM } variant;
    float jumpTimer = 0.f;

    Frog(uint32_t id, glm::vec3 p, Variant v = Variant::TEMPERATE)
        : Animal(id, "Rana"), variant(v)
    {
        pos = p; maxHealth = health = 10;
        bodyW = 0.5f; bodyH = 0.55f;
        walkSpeed = 1.5f; fleeSpeed = 5.0f;
        drops = {};
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if ((jumpTimer -= dt) <= 0.f && onGround) {
            velocity.y = 5.0f; onGround = false;
            jumpTimer = 1.5f + (float)(std::rand() % 3);
        }
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        switch (variant) {
            case Variant::TEMPERATE: return {0.88f, 0.60f, 0.35f}; // naranja
            case Variant::COLD:      return {0.35f, 0.65f, 0.25f}; // verde
            case Variant::WARM:      return {0.85f, 0.82f, 0.65f}; // crema
        } return {0.88f, 0.60f, 0.35f};
    }
    glm::vec3 legColor() const override { return bodyColor() * 0.75f; }
    const char* soundAmbient() const override { return "frog_croak"; }
    const char* soundHurt()    const override { return "frog_hurt";  }
    const char* soundDeath()   const override { return "frog_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  RENACUAJO  —  Sale de huevos. Se convierte en rana al crecer.
// ─────────────────────────────────────────────────────────────────────────────
class Tadpole : public AquaticAnimal {
public:
    float growTimer;
    Tadpole(uint32_t id, glm::vec3 p) : AquaticAnimal(id, "Renacuajo") {
        pos = p; maxHealth = health = 6;
        bodyW = 0.3f; bodyH = 0.3f;
        walkSpeed = 2.5f; fleeSpeed = 5.0f;
        growTimer = 40.f + (float)(std::rand() % 20);
        drops = {};
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        growTimer -= dt;  // cuando llega a 0 → transformar en Frog
        return AquaticAnimal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.25f, 0.48f, 0.25f}; }
    glm::vec3 legColor()  const override { return {0.20f, 0.40f, 0.20f}; }
    const char* soundAmbient() const override { return "tadpole_ambient"; }
    const char* soundHurt()    const override { return "tadpole_hurt";    }
    const char* soundDeath()   const override { return "tadpole_death";   }
};
