#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  EndMobs.h  –  Criaturas del End + mobs especiales (1.21, Warden, etc.)
// ─────────────────────────────────────────────────────────────────────────────
#include "Entity.h"

// ── End ───────────────────────────────────────────────────────────────────────
inline EntityDef END_MOBS[] = {
    {
        "Shulker",
        EntityType::SHULKER,
        EntityBehavior::HOSTILE,
        EntityDimension::END,
        30.f, 4.f, 0.0f,        // estático, dispara proyectiles que leviatan
        "Concha de shulker"
    },
    {
        "Ender Dragon",
        EntityType::ENDER_DRAGON,
        EntityBehavior::BOSS,
        EntityDimension::END,
        200.f, 10.f, 0.60f,
        "Huevo de dragón, Experiencia masiva"
    },
};

inline constexpr int END_MOB_COUNT =
    sizeof(END_MOBS) / sizeof(END_MOBS[0]);

// ── Cuevas profundas (Deep Dark, Y < 0) ──────────────────────────────────────
inline EntityDef CAVE_MOBS[] = {
    {
        "Warden",
        EntityType::WARDEN,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        500.f, 30.f, 0.30f,     // ciego, detecta por sonido/vibración
        "Fragmento de eco"
    },
};

inline constexpr int CAVE_MOB_COUNT =
    sizeof(CAVE_MOBS) / sizeof(CAVE_MOBS[0]);

// ── Monstruos nuevos 1.21 (Trial Chambers) ───────────────────────────────────
inline EntityDef NEW_MOBS[] = {
    {
        "Bogged",
        EntityType::BOGGED,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        16.f, 2.f, 0.25f,       // esqueleto pantanoso, dispara flechas de veneno
        "Flechas de veneno, Huesos"
    },
    {
        "Breeze",
        EntityType::BREEZE,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        30.f, 1.f, 0.60f,       // flota, dispara ráfagas de viento que empujan
        "Vara de breeze"
    },
};

inline constexpr int NEW_MOB_COUNT =
    sizeof(NEW_MOBS) / sizeof(NEW_MOBS[0]);

// ── Helper: ¿es un jefe (boss)? ──────────────────────────────────────────────
inline bool isBoss(EntityType t) {
    return t == EntityType::ENDER_DRAGON || t == EntityType::WARDEN;
}

// ── Helper: ¿puede spawnar de noche en superficie? ───────────────────────────
inline bool spawnsAtNight(EntityType t) {
    switch (t) {
        case EntityType::ZOMBIE:
        case EntityType::SKELETON:
        case EntityType::SPIDER:
        case EntityType::CREEPER:
        case EntityType::ENDERMAN:
        case EntityType::PHANTOM:
            return true;
        default:
            return false;
    }
}
