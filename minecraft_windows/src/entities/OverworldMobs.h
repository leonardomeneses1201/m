#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  OverworldMobs.h  –  Criaturas hostiles del Overworld (mundo normal)
// ─────────────────────────────────────────────────────────────────────────────
#include "Entity.h"

// ── Tabla de monstruos del Overworld ─────────────────────────────────────────
//  { nombre, tipo, comportamiento, dimensión, HP, daño, velocidad, botín }

inline EntityDef OVERWORLD_MOBS[] = {
    {
        "Zombie",
        EntityType::ZOMBIE,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 3.f, 0.23f,
        "Carne podrida"
    },
    {
        "Skeleton",
        EntityType::SKELETON,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 2.f, 0.25f,
        "Flechas, Huesos"
    },
    {
        "Spider",
        EntityType::SPIDER,
        EntityBehavior::NEUTRAL,    // neutral de dia, hostil de noche
        EntityDimension::OVERWORLD,
        16.f, 2.f, 0.30f,
        "Hilo, Ojo de araña"
    },
    {
        "Cave Spider",
        EntityType::CAVE_SPIDER,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        12.f, 2.f, 0.30f,
        "Hilo, Ojo de araña"
    },
    {
        "Creeper",
        EntityType::CREEPER,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 0.f,  // daño por explosion, no golpe
        0.25f,
        "Polvo de pólvora"
    },
    {
        "Enderman",
        EntityType::ENDERMAN,
        EntityBehavior::NEUTRAL,    // hostil si lo miras a los ojos
        EntityDimension::ALL,
        40.f, 7.f, 0.30f,
        "Perla de ender"
    },
    {
        "Witch",
        EntityType::WITCH,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        26.f, 0.f,  // ataca con pociones
        0.25f,
        "Palos, Redstone, Pociones"
    },
    {
        "Zombie Villager",
        EntityType::ZOMBIE_VILLAGER,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 3.f, 0.23f,
        "Carne podrida"
    },
    {
        "Skeleton Horse",
        EntityType::SKELETON_HORSE,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        15.f, 0.f, 0.20f,
        "Huesos"
    },
    // ── Variantes nevadas / áridas ────────────────────────────────────────────
    {
        "Husk",
        EntityType::HUSK,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 3.f, 0.23f,
        "Carne podrida"          // en desiertos, causa hambre
    },
    {
        "Stray",
        EntityType::STRAY,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 2.f, 0.25f,
        "Flechas de lentitud, Huesos"
    },
    {
        "Drowned",
        EntityType::DROWNED,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 2.5f, 0.20f,
        "Carne podrida, Tridente (raro)"
    },
    {
        "Phantom",
        EntityType::PHANTOM,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        20.f, 6.f, 0.50f,       // aparece si no duermes en 3+ días
        "Membrana de phantom"
    },
    {
        "Slime",
        EntityType::SLIME,
        EntityBehavior::HOSTILE,
        EntityDimension::OVERWORLD,
        16.f, 3.f, 0.20f,
        "Bola de slime"          // se divide al morir
    },
};

inline constexpr int OVERWORLD_MOB_COUNT =
    sizeof(OVERWORLD_MOBS) / sizeof(OVERWORLD_MOBS[0]);

// ── Helper: buscar definición por tipo ───────────────────────────────────────
inline const EntityDef* findOverworldMob(EntityType t) {
    for (int i = 0; i < OVERWORLD_MOB_COUNT; i++)
        if (OVERWORLD_MOBS[i].type == t) return &OVERWORLD_MOBS[i];
    return nullptr;
}
