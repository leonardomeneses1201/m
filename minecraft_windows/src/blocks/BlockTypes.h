#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  BlockTypes.h  –  Enumeración extendida de bloques y utilidades de consulta
//  Incluye: categorías, propiedades físicas y helpers de clasificación.
// ─────────────────────────────────────────────────────────────────────────────
#include "Block.h"

// ── Categorías de bloque ──────────────────────────────────────────────────────
enum class BlockCategory {
    NONE,
    TERRAIN,    // Grass, Dirt, Stone, Sand, Bedrock
    ORGANIC,    // Wood, Leaves
    FLUID,      // Water
};

// ── Resistencia a la explosión (varía por tipo) ───────────────────────────────
inline float blastResistance(BlockType t) {
    switch (t) {
        case BlockType::BEDROCK: return 1e6f;   // indestructible
        case BlockType::STONE:   return 6.0f;
        case BlockType::DIRT:
        case BlockType::GRASS:   return 0.5f;
        case BlockType::SAND:    return 0.5f;
        case BlockType::WOOD:    return 2.0f;
        case BlockType::LEAVES:  return 0.2f;
        case BlockType::WATER:   return 100.0f; // fluido, no destruible por explosión
        default:                 return 1.0f;
    }
}

// ── Dureza (tiempo base en segundos para romper a mano) ───────────────────────
inline float blockHardness(BlockType t) {
    switch (t) {
        case BlockType::BEDROCK: return -1.f;   // no rompible
        case BlockType::STONE:   return 1.5f;
        case BlockType::DIRT:
        case BlockType::GRASS:   return 0.5f;
        case BlockType::SAND:    return 0.5f;
        case BlockType::WOOD:    return 2.0f;
        case BlockType::LEAVES:  return 0.2f;
        case BlockType::WATER:   return 100.f;
        default:                 return 1.0f;
    }
}

// ── Categoría ─────────────────────────────────────────────────────────────────
inline BlockCategory blockCategory(BlockType t) {
    switch (t) {
        case BlockType::GRASS:
        case BlockType::DIRT:
        case BlockType::STONE:
        case BlockType::SAND:
        case BlockType::BEDROCK:  return BlockCategory::TERRAIN;
        case BlockType::WOOD:
        case BlockType::LEAVES:   return BlockCategory::ORGANIC;
        case BlockType::WATER:    return BlockCategory::FLUID;
        default:                  return BlockCategory::NONE;
    }
}

// ── ¿Puede el bloque recibir luz solar encima? ────────────────────────────────
inline bool isOpaque(BlockType t) {
    return isSolid(t) && !isTransparent(t);
}

// ── ¿El bloque es fluido? ─────────────────────────────────────────────────────
inline bool isFluid(BlockType t) {
    return t == BlockType::WATER;
}

// ── ¿Es un bloque natural de superficie? ─────────────────────────────────────
inline bool isSurface(BlockType t) {
    return t == BlockType::GRASS || t == BlockType::SAND;
}
