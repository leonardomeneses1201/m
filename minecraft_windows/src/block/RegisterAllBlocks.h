#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  RegisterAllBlocks.h  –  Registra todos los bloques de una sola llamada
//  Uso en Game::init():
//      #include "block/RegisterAllBlocks.h"
//      registerAllBlocks();
// ─────────────────────────────────────────────────────────────────────────────
#include "WoodBlocks.h"
#include "StoneBlocks.h"
#include "MetalBlocks.h"

inline void registerAllBlocks() {
    registerWoodBlocks();           // ~120 bloques de madera
    registerStoneBlocks();          // ~80  bloques de piedra
    registerMetalOreBlocks();       // ~20  menas y minerales
    registerMetalStorageBlocks();   // ~18  bloques de almacenamiento/cuarzo
    registerCopperVariants();       // ~30  variantes de cobre
}
