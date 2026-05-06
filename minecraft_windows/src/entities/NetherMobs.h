#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  NetherMobs.h  –  Criaturas del Nether (infierno)
// ─────────────────────────────────────────────────────────────────────────────
#include "Entity.h"

inline EntityDef NETHER_MOBS[] = {
    {
        "Zombified Piglin",
        EntityType::ZOMBIFIED_PIGLIN,
        EntityBehavior::NEUTRAL,    // hostil solo si lo atacas
        EntityDimension::NETHER,
        20.f, 5.f, 0.23f,
        "Pepitas de oro, Carne cocida"
    },
    {
        "Piglin",
        EntityType::PIGLIN,
        EntityBehavior::NEUTRAL,    // neutral con armadura dorada puesta
        EntityDimension::NETHER,
        16.f, 5.f, 0.25f,
        "Espada dorada, Armadura dorada"
    },
    {
        "Piglin Brute",
        EntityType::PIGLIN_BRUTE,
        EntityBehavior::HOSTILE,    // siempre hostil, ignora el oro
        EntityDimension::NETHER,
        50.f, 7.f, 0.35f,
        "Hacha dorada"
    },
    {
        "Wither Skeleton",
        EntityType::WITHER_SKELETON,
        EntityBehavior::HOSTILE,
        EntityDimension::NETHER,
        20.f, 4.f, 0.25f,
        "Carbón, Huesos, Cabeza wither (muy raro)"
    },
    {
        "Blaze",
        EntityType::BLAZE,
        EntityBehavior::HOSTILE,
        EntityDimension::NETHER,
        20.f, 6.f, 0.23f,
        "Vara de blaze"
    },
    {
        "Ghast",
        EntityType::GHAST,
        EntityBehavior::HOSTILE,
        EntityDimension::NETHER,
        10.f, 6.f, 0.40f,       // flota, dispara bolas de fuego explosivas
        "Lágrima de ghast, Pólvora"
    },
    {
        "Magma Cube",
        EntityType::MAGMA_CUBE,
        EntityBehavior::HOSTILE,
        EntityDimension::NETHER,
        16.f, 3.f, 0.20f,
        "Crema de magma"         // se divide al morir como los slimes
    },
    {
        "Hoglin",
        EntityType::HOGLIN,
        EntityBehavior::NEUTRAL,
        EntityDimension::NETHER,
        40.f, 6.f, 0.30f,
        "Carne de cerdo cocida, Cuero"
    },
};

inline constexpr int NETHER_MOB_COUNT =
    sizeof(NETHER_MOBS) / sizeof(NETHER_MOBS[0]);

inline const EntityDef* findNetherMob(EntityType t) {
    for (int i = 0; i < NETHER_MOB_COUNT; i++)
        if (NETHER_MOBS[i].type == t) return &NETHER_MOBS[i];
    return nullptr;
}
