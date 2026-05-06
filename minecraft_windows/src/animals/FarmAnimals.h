#pragma once
#include "Animal.h"
#include <cmath>
#include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
//  VACA  —  Drops: Carne cruda, Cuero. Cría: Trigo. Da leche con cubo.
// ─────────────────────────────────────────────────────────────────────────────
class Cow : public Animal {
public:
    Cow(uint32_t id, glm::vec3 p) : Animal(id, "Vaca") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.9f; bodyH = 1.4f;
        walkSpeed = 2.0f; fleeSpeed = 4.5f;
        drops = { {"Carne Cruda", 1, 3}, {"Cuero", 0, 2} };
    }
    glm::vec3 bodyColor() const override { return {0.35f, 0.22f, 0.12f}; }
    glm::vec3 legColor()  const override { return {0.20f, 0.12f, 0.06f}; }
    const char* soundAmbient() const override { return "cow_moo";   }
    const char* soundHurt()    const override { return "cow_hurt";  }
    const char* soundDeath()   const override { return "cow_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CERDO  —  Drops: Chuleta cruda. Cría: Zanahoria / Patata.
// ─────────────────────────────────────────────────────────────────────────────
class Pig : public Animal {
public:
    Pig(uint32_t id, glm::vec3 p) : Animal(id, "Cerdo") {
        pos = p; maxHealth = health = 10;
        bodyW = 0.75f; bodyH = 0.9f;
        walkSpeed = 2.5f; fleeSpeed = 5.0f;
        drops = { {"Chuleta Cruda", 1, 3} };
    }
    glm::vec3 bodyColor() const override { return {0.96f, 0.70f, 0.70f}; }
    glm::vec3 legColor()  const override { return {0.88f, 0.55f, 0.55f}; }
    const char* soundAmbient() const override { return "pig_oink";  }
    const char* soundHurt()    const override { return "pig_hurt";  }
    const char* soundDeath()   const override { return "pig_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  OVEJA  —  Drops: Lana, Cordero. 5 colores de lana. Cría: Trigo.
// ─────────────────────────────────────────────────────────────────────────────
class Sheep : public Animal {
public:
    enum class WoolColor : uint8_t { WHITE, GRAY, BLACK, BROWN, PINK } woolColor;
    bool  sheared = false;
    float regrowTimer = 0.f;

    Sheep(uint32_t id, glm::vec3 p) : Animal(id, "Oveja") {
        pos = p; maxHealth = health = 8;
        bodyW = 0.9f; bodyH = 1.3f;
        walkSpeed = 2.0f; fleeSpeed = 4.8f;
        drops = { {"Lana", 1, 3}, {"Cordero Crudo", 1, 2} };
        int r = std::rand() % 100;
        woolColor = r < 82 ? WoolColor::WHITE :
                    r < 87 ? WoolColor::BROWN :
                    r < 92 ? WoolColor::GRAY  :
                    r < 97 ? WoolColor::BLACK : WoolColor::PINK;
    }
    void shear() { if (!sheared) { sheared = true; regrowTimer = 60.f; } }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if (sheared && (regrowTimer -= dt) <= 0.f) sheared = false;
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override {
        if (sheared) return {0.55f, 0.50f, 0.45f};
        switch (woolColor) {
            case WoolColor::WHITE: return {0.96f, 0.96f, 0.96f};
            case WoolColor::GRAY:  return {0.55f, 0.55f, 0.55f};
            case WoolColor::BLACK: return {0.12f, 0.12f, 0.12f};
            case WoolColor::BROWN: return {0.48f, 0.28f, 0.12f};
            case WoolColor::PINK:  return {0.97f, 0.60f, 0.72f};
        } return {0.96f, 0.96f, 0.96f};
    }
    glm::vec3 headColor() const override { return {0.55f, 0.52f, 0.50f}; }
    glm::vec3 legColor()  const override { return {0.40f, 0.38f, 0.36f}; }
    const char* soundAmbient() const override { return "sheep_baa";   }
    const char* soundHurt()    const override { return "sheep_hurt";  }
    const char* soundDeath()   const override { return "sheep_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  GALLINA  —  Drops: Pollo, Pluma. Pone huevos. Cría: Semillas.
// ─────────────────────────────────────────────────────────────────────────────
class Chicken : public Animal {
public:
    float eggTimer;
    Chicken(uint32_t id, glm::vec3 p) : Animal(id, "Gallina") {
        pos = p; maxHealth = health = 4;
        bodyW = 0.4f; bodyH = 0.7f;
        walkSpeed = 2.8f; fleeSpeed = 5.8f;
        eggTimer = 300.f + (float)(std::rand() % 300);
        drops = { {"Pollo Crudo", 1, 1}, {"Pluma", 0, 2} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if (!onGround && velocity.y < 0.f)
            velocity.y = std::max(velocity.y, -2.0f);   // caída lenta
        if ((eggTimer -= dt) <= 0.f) eggTimer = 300.f + (float)(std::rand()%300);
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.94f, 0.94f, 0.88f}; }
    glm::vec3 legColor()  const override { return {0.95f, 0.65f, 0.10f}; }
    const char* soundAmbient() const override { return "chicken_cluck"; }
    const char* soundHurt()    const override { return "chicken_hurt";  }
    const char* soundDeath()   const override { return "chicken_death"; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  CONEJO  —  Drops: Carne, Piel, Pata de conejo. Se mueve a saltos.
// ─────────────────────────────────────────────────────────────────────────────
class Rabbit : public Animal {
public:
    float hopTimer = 0.f;
    Rabbit(uint32_t id, glm::vec3 p) : Animal(id, "Conejo") {
        pos = p; maxHealth = health = 3;
        bodyW = 0.4f; bodyH = 0.5f;
        walkSpeed = 3.5f; fleeSpeed = 7.5f;
        drops = { {"Conejo Crudo", 1, 1}, {"Piel de Conejo", 0, 1}, {"Pata de Conejo", 0, 1} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        if ((hopTimer -= dt) <= 0.f && onGround &&
            (state == AnimalState::WALKING || state == AnimalState::FLEEING)) {
            velocity.y = 5.5f; onGround = false; hopTimer = 0.55f;
        }
        return Animal::update(dt, pp, w);
    }
    glm::vec3 bodyColor() const override { return {0.75f, 0.60f, 0.45f}; }
    glm::vec3 legColor()  const override { return {0.65f, 0.50f, 0.35f}; }
    const char* soundAmbient() const override { return "rabbit_squeak"; }
    const char* soundHurt()    const override { return "rabbit_hurt";   }
    const char* soundDeath()   const override { return "rabbit_death";  }
};

// ─────────────────────────────────────────────────────────────────────────────
//  ABEJA  —  Vuela. Drops: Miel/Panal. Ataca si provocas la colmena.
// ─────────────────────────────────────────────────────────────────────────────
class Bee : public Animal {
public:
    glm::vec3 hivePos;
    float     buzzTimer = 0.f;
    bool      hasStung  = false;

    Bee(uint32_t id, glm::vec3 p, glm::vec3 hive)
        : Animal(id, "Abeja"), hivePos(hive)
    {
        pos = p; maxHealth = health = 10;
        bodyW = 0.55f; bodyH = 0.5f;
        walkSpeed = 3.0f; fleeSpeed = 4.5f;
        drops = { {"Miel", 0, 1} };
    }
    bool update(float dt, const glm::vec3& pp, World& w) override {
        velocity.y = std::sin(buzzTimer) * 0.5f;
        onGround   = false;
        buzzTimer += dt * 2.5f;
        float distToHive = glm::length(pos - hivePos);
        if (distToHive > 22.f && state == AnimalState::IDLE) {
            glm::vec3 dir = glm::normalize(hivePos - pos);
            yaw = glm::degrees(std::atan2(dir.z, dir.x));
            state = AnimalState::WALKING; stateTimer = 4.f;
        }
        return Animal::update(dt, pp, w);
    }
    void damage(int amount) override {
        Animal::damage(amount);
        if (alive && !hasStung) hasStung = true;
    }
    glm::vec3 bodyColor() const override { return {0.95f, 0.80f, 0.10f}; }
    glm::vec3 legColor()  const override { return {0.10f, 0.08f, 0.05f}; }
    const char* soundAmbient() const override { return "bee_buzz"; }
    const char* soundHurt()    const override { return "bee_hurt"; }
    const char* soundDeath()   const override { return "bee_death";}
};
