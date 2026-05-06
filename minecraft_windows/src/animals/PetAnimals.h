#pragma once
#include "Animal.h"
#include <cmath>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
//  LOBO  —  Domesticar: Huesos (prob. 1/3). Defiende al dueño.
// ─────────────────────────────────────────────────────────────────────────────
class Wolf : public Animal {
public:
    bool  tamed   = false;
    bool  sitting = false;
    float attackCooldown = 0.f;

    Wolf(uint32_t id, glm::vec3 p) : Animal(id, "Lobo") {
        pos = p; maxHealth = health = 20;
        bodyW = 0.8f; bodyH = 0.85f;
        walkSpeed = 3.5f; fleeSpeed = 8.0f;
    }
    void tryTame() {
        if (!tamed && std::rand() % 3 == 0) { tamed = true; health = maxHealth; }
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        attackCooldown -= dt;
        if (tamed && !sitting) state = AnimalState::FOLLOWING;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        return tamed ? glm::vec3{0.72f,0.55f,0.35f} : glm::vec3{0.55f,0.55f,0.55f};
    }
    glm::vec3 legColor() const override { return bodyColor(); }
    const char* soundAmbient() const override { return tamed ? "wolf_whine" : "wolf_growl"; }
    const char* soundHurt()    const override { return "wolf_hurt";  }
    const char* soundDeath()   const override { return "wolf_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  GATO  —  Domesticar: Pescado crudo. Ahuyenta Creepers y Phantoms.
// ─────────────────────────────────────────────────────────────────────────────
class Cat : public Animal {
public:
    bool tamed = false;
    enum class Fur : uint8_t { ORANGE, BLACK, WHITE, TABBY, SIAMESE } fur;

    Cat(uint32_t id, glm::vec3 p) : Animal(id, "Gato") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.45f; bodyH = 0.7f;
        walkSpeed = 3.0f; fleeSpeed = 6.5f;
        fur = (Fur)(std::rand() % 5);
        drops = {};
    }
    void tryTame() {
        if (!tamed && std::rand() % 3 == 0) { tamed = true; health = maxHealth; }
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if (tamed) state = AnimalState::FOLLOWING;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        switch (fur) {
            case Fur::ORANGE:  return {0.95f, 0.55f, 0.10f};
            case Fur::BLACK:   return {0.10f, 0.10f, 0.10f};
            case Fur::WHITE:   return {0.98f, 0.98f, 0.98f};
            case Fur::TABBY:   return {0.55f, 0.40f, 0.25f};
            case Fur::SIAMESE: return {0.90f, 0.85f, 0.75f};
        } return {0.80f, 0.70f, 0.60f};
    }
    glm::vec3 legColor() const override { return bodyColor() * 0.85f; }
    const char* soundAmbient() const override { return tamed ? "cat_purr" : "cat_meow"; }
    const char* soundHurt()    const override { return "cat_hurt";  }
    const char* soundDeath()   const override { return "cat_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  LORO  —  Domesticar: Semillas. Se sienta en el hombro. Imita sonidos.
// ─────────────────────────────────────────────────────────────────────────────
class Parrot : public Animal {
public:
    bool  tamed      = false;
    bool  onShoulder = false;
    float wingFlap   = 0.f;
    enum class Color : uint8_t { RED, BLUE, GREEN, CYAN, GRAY } color;

    Parrot(uint32_t id, glm::vec3 p) : Animal(id, "Loro") {
        pos = p; maxHealth = health = 6;
        bodyW = 0.35f; bodyH = 0.75f;
        walkSpeed = 0.f; fleeSpeed = 5.0f;
        color = (Color)(std::rand() % 5);
        drops = { {"Pluma", 1, 2} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        velocity.y = std::sin(wingFlap) * 0.6f;
        onGround   = false;
        wingFlap  += dt * 8.f;
        if (tamed && !onShoulder) state = AnimalState::FOLLOWING;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        switch (color) {
            case Color::RED:   return {0.95f, 0.15f, 0.10f};
            case Color::BLUE:  return {0.15f, 0.40f, 0.90f};
            case Color::GREEN: return {0.10f, 0.75f, 0.20f};
            case Color::CYAN:  return {0.10f, 0.80f, 0.80f};
            case Color::GRAY:  return {0.55f, 0.55f, 0.55f};
        } return {0.80f, 0.20f, 0.10f};
    }
    glm::vec3 legColor() const override { return {0.95f, 0.65f, 0.10f}; }
    const char* soundAmbient() const override { return "parrot_squawk"; }
    const char* soundHurt()    const override { return "parrot_hurt";   }
    const char* soundDeath()   const override { return "parrot_death";  }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CABALLO  —  Domar: montarlo repetidamente. Estadísticas aleatorias.
// ─────────────────────────────────────────────────────────────────────────────
class Horse : public Animal {
public:
    bool  tamed = false, saddled = false;
    int   tameTries = 0;
    float jumpStrength, maxSpeed;
    enum class Coat : uint8_t { WHITE,GRAY,BROWN,DARKBROWN,BLACK,CHESTNUT,CREAMY } coat;

    Horse(uint32_t id, glm::vec3 p) : Animal(id, "Caballo") {
        pos = p; maxHealth = health = 20 + std::rand() % 10;
        bodyW = 1.3f; bodyH = 1.6f;
        jumpStrength = 0.4f + (float)(std::rand() % 61) / 100.f;
        maxSpeed     = 5.0f + (float)(std::rand() % 8);
        walkSpeed    = 3.0f; fleeSpeed = 8.0f;
        coat = (Coat)(std::rand() % 7);
        drops = { {"Cuero", 0, 2} };
    }
    void tryMount() {
        tameTries++;
        if (std::rand() % 100 < 5 + tameTries * 10) {
            tamed = true; walkSpeed = maxSpeed;
        }
    }
    glm::vec3 bodyColor() const override {
        switch (coat) {
            case Coat::WHITE:     return {0.95f, 0.95f, 0.92f};
            case Coat::GRAY:      return {0.60f, 0.60f, 0.60f};
            case Coat::BROWN:     return {0.60f, 0.38f, 0.18f};
            case Coat::DARKBROWN: return {0.32f, 0.18f, 0.08f};
            case Coat::BLACK:     return {0.10f, 0.10f, 0.10f};
            case Coat::CHESTNUT:  return {0.72f, 0.30f, 0.08f};
            case Coat::CREAMY:    return {0.92f, 0.82f, 0.62f};
        } return {0.60f, 0.38f, 0.18f};
    }
    glm::vec3 legColor()  const override { return bodyColor() * 0.75f; }
    const char* soundAmbient() const override { return "horse_neigh"; }
    const char* soundHurt()    const override { return "horse_hurt";  }
    const char* soundDeath()   const override { return "horse_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  BURRO  —  Más lento que el caballo. Puede llevar cofre.
// ─────────────────────────────────────────────────────────────────────────────
class Donkey : public Animal {
public:
    bool tamed = false, saddled = false, hasChest = false;
    Donkey(uint32_t id, glm::vec3 p) : Animal(id, "Burro") {
        pos = p; maxHealth = health = 15 + std::rand() % 8;
        bodyW = 1.2f; bodyH = 1.5f;
        walkSpeed = 4.0f; fleeSpeed = 6.5f;
        drops = { {"Cuero", 0, 2} };
    }
    glm::vec3 bodyColor() const override { return {0.55f, 0.52f, 0.45f}; }
    glm::vec3 legColor()  const override { return {0.45f, 0.42f, 0.36f}; }
    const char* soundAmbient() const override { return "donkey_bray";  }
    const char* soundHurt()    const override { return "donkey_hurt";  }
    const char* soundDeath()   const override { return "donkey_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  MULA  —  Híbrido Caballo+Burro. No puede criar. Lleva cofre.
// ─────────────────────────────────────────────────────────────────────────────
class Mule : public Animal {
public:
    bool tamed = false, hasChest = false;
    Mule(uint32_t id, glm::vec3 p) : Animal(id, "Mula") {
        pos = p; maxHealth = health = 22 + std::rand() % 8;
        bodyW = 1.25f; bodyH = 1.55f;
        walkSpeed = 4.5f; fleeSpeed = 7.0f;
        drops = { {"Cuero", 0, 2} };
    }
    glm::vec3 bodyColor() const override { return {0.28f, 0.20f, 0.12f}; }
    glm::vec3 legColor()  const override { return {0.20f, 0.15f, 0.08f}; }
    const char* soundAmbient() const override { return "mule_bray";  }
    const char* soundHurt()    const override { return "mule_hurt";  }
    const char* soundDeath()   const override { return "mule_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CAMELLO  —  Lleva 2 jugadores. Dash especial. Aldeas del desierto.
// ─────────────────────────────────────────────────────────────────────────────
class Camel : public Animal {
public:
    bool  saddled = false;
    float dashCooldown = 0.f;
    Camel(uint32_t id, glm::vec3 p) : Animal(id, "Camello") {
        pos = p; maxHealth = health = 32;
        bodyW = 1.7f; bodyH = 2.3f;
        walkSpeed = 3.5f; fleeSpeed = 7.5f;
        drops = {};
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        dashCooldown -= dt;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.86f, 0.72f, 0.42f}; }
    glm::vec3 legColor()  const override { return {0.78f, 0.62f, 0.32f}; }
    const char* soundAmbient() const override { return "camel_ambient"; }
    const char* soundHurt()    const override { return "camel_hurt";    }
    const char* soundDeath()   const override { return "camel_death";   }
};

// ─────────────────────────────────────────────────────────────────────────────
//  LLAMA  —  Forma caravanas. Escupe a lobos. Cofre de 3-15 slots.
// ─────────────────────────────────────────────────────────────────────────────
class Llama : public Animal {
public:
    bool  tamed = false, hasChest = false;
    int   chestSlots;
    float spitCooldown = 0.f;
    enum class Color : uint8_t { CREAMY, WHITE, BROWN, GRAY } color;

    Llama(uint32_t id, glm::vec3 p) : Animal(id, "Llama") {
        pos = p; maxHealth = health = 15 + std::rand() % 16;
        bodyW = 1.0f; bodyH = 1.9f;
        walkSpeed = 3.0f; fleeSpeed = 5.5f;
        chestSlots = 3 + std::rand() % 13;
        color = (Color)(std::rand() % 4);
        drops = { {"Cuero", 0, 2} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        spitCooldown -= dt;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        switch (color) {
            case Color::CREAMY: return {0.92f, 0.84f, 0.68f};
            case Color::WHITE:  return {0.95f, 0.95f, 0.92f};
            case Color::BROWN:  return {0.52f, 0.35f, 0.20f};
            case Color::GRAY:   return {0.58f, 0.58f, 0.58f};
        } return {0.90f, 0.82f, 0.65f};
    }
    glm::vec3 legColor() const override { return bodyColor() * 0.80f; }
    const char* soundAmbient() const override { return "llama_ambient"; }
    const char* soundHurt()    const override { return "llama_hurt";    }
    const char* soundDeath()   const override { return "llama_death";   }
};
