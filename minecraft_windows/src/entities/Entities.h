#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Entities.h  –  Cabecera maestra: incluye todas las criaturas
//  Uso: #include "entities/Entities.h"
// ─────────────────────────────────────────────────────────────────────────────
#include "Entity.h"
#include "OverworldMobs.h"
#include "NetherMobs.h"
#include "EndMobs.h"

// ── Busca una definición en todas las tablas ──────────────────────────────────
inline const EntityDef* findEntityDef(EntityType t) {
    if (auto* d = findOverworldMob(t)) return d;
    if (auto* d = findNetherMob(t))   return d;
    for (int i = 0; i < END_MOB_COUNT;  i++) if (END_MOBS[i].type  == t) return &END_MOBS[i];
    for (int i = 0; i < CAVE_MOB_COUNT; i++) if (CAVE_MOBS[i].type == t) return &CAVE_MOBS[i];
    for (int i = 0; i < NEW_MOB_COUNT;  i++) if (NEW_MOBS[i].type  == t) return &NEW_MOBS[i];
    return nullptr;
}

// ── Total de criaturas registradas ────────────────────────────────────────────
inline constexpr int TOTAL_MOB_COUNT =
    OVERWORLD_MOB_COUNT + NETHER_MOB_COUNT +
    END_MOB_COUNT + CAVE_MOB_COUNT + NEW_MOB_COUNT;
