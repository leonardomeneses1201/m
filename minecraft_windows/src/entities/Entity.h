#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Entity.h  –  Clase base para todas las criaturas del mundo
// ─────────────────────────────────────────────────────────────────────────────
#include <glm/glm.hpp>
#include <string>

enum class EntityType {
    // ── Overworld ─────────────────────────────
    ZOMBIE, SKELETON, SPIDER, CAVE_SPIDER,
    CREEPER, ENDERMAN, WITCH,
    ZOMBIE_VILLAGER, SKELETON_HORSE,
    // ── Nether ────────────────────────────────
    ZOMBIFIED_PIGLIN, PIGLIN, PIGLIN_BRUTE,
    WITHER_SKELETON, BLAZE, GHAST,
    MAGMA_CUBE, HOGLIN,
    // ── End ───────────────────────────────────
    SHULKER, ENDER_DRAGON,
    // ── Cuevas profundas ──────────────────────
    WARDEN,
    // ── Evento ────────────────────────────────
    PHANTOM, SLIME, DROWNED, HUSK, STRAY,
    // ── 1.21 ──────────────────────────────────
    BOGGED, BREEZE,
};

enum class EntityBehavior {
    PASSIVE,    // nunca ataca
    NEUTRAL,    // ataca solo si es provocado
    HOSTILE,    // ataca al ver al jugador
    BOSS,       // jefe con mecánicas especiales
};

enum class EntityDimension {
    OVERWORLD,
    NETHER,
    END,
    ALL,        // aparece en varias dimensiones (ej. Enderman)
};

struct EntityDef {
    const char*     name;
    EntityType      type;
    EntityBehavior  behavior;
    EntityDimension dimension;
    float           maxHealth;
    float           attackDamage;
    float           moveSpeed;
    const char*     loot;       // botín principal
};
