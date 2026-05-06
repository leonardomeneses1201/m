#include "BlockRegistry.h"
// ─────────────────────────────────────────────────────────────────────────────
//  MetalBlocks.cpp  –  Minerales, bloques de almacenamiento y variantes de cobre
// ─────────────────────────────────────────────────────────────────────────────

// ── 1. MENAS (minerales en roca) ─────────────────────────────────────────────
void registerMetalOreBlocks() {
    auto& reg = BlockRegistry::get();
    BlockProperties p;
    p.solid = true; p.transparent = false; p.gravity = false;
    p.fluid = false; p.opaque = true;
    p.hardness = 3.0f; p.blastResistance = 3.0f;

    // Carbón
    p.name = "Coal Ore";
    p.texTop = {0,3}; p.texSide = {0,3}; p.texBottom = {0,3};
    reg.registerBlock(p);
    p.name = "Deepslate Coal Ore";
    p.texTop = {0,4}; p.texSide = {0,4}; p.texBottom = {0,4};
    reg.registerBlock(p);

    // Hierro
    p.name = "Iron Ore";
    p.texTop = {1,3}; p.texSide = {1,3}; p.texBottom = {1,3};
    reg.registerBlock(p);
    p.name = "Deepslate Iron Ore";
    p.texTop = {1,4}; p.texSide = {1,4}; p.texBottom = {1,4};
    reg.registerBlock(p);

    // Cobre
    p.name = "Copper Ore";
    p.texTop = {2,3}; p.texSide = {2,3}; p.texBottom = {2,3};
    reg.registerBlock(p);
    p.name = "Deepslate Copper Ore";
    p.texTop = {2,4}; p.texSide = {2,4}; p.texBottom = {2,4};
    reg.registerBlock(p);

    // Oro
    p.name = "Gold Ore";
    p.texTop = {3,3}; p.texSide = {3,3}; p.texBottom = {3,3};
    reg.registerBlock(p);
    p.name = "Deepslate Gold Ore";
    p.texTop = {3,4}; p.texSide = {3,4}; p.texBottom = {3,4};
    reg.registerBlock(p);
    p.name = "Nether Gold Ore";
    p.texTop = {3,5}; p.texSide = {3,5}; p.texBottom = {3,5};
    reg.registerBlock(p);

    // Redstone
    p.name = "Redstone Ore";
    p.texTop = {4,3}; p.texSide = {4,3}; p.texBottom = {4,3};
    reg.registerBlock(p);
    p.name = "Deepslate Redstone Ore";
    p.texTop = {4,4}; p.texSide = {4,4}; p.texBottom = {4,4};
    reg.registerBlock(p);

    // Esmeralda
    p.name = "Emerald Ore";
    p.texTop = {5,3}; p.texSide = {5,3}; p.texBottom = {5,3};
    reg.registerBlock(p);
    p.name = "Deepslate Emerald Ore";
    p.texTop = {5,4}; p.texSide = {5,4}; p.texBottom = {5,4};
    reg.registerBlock(p);

    // Lapislázuli
    p.name = "Lapis Lazuli Ore";
    p.texTop = {6,3}; p.texSide = {6,3}; p.texBottom = {6,3};
    reg.registerBlock(p);
    p.name = "Deepslate Lapis Lazuli Ore";
    p.texTop = {6,4}; p.texSide = {6,4}; p.texBottom = {6,4};
    reg.registerBlock(p);

    // Diamante
    p.name = "Diamond Ore";
    p.texTop = {7,3}; p.texSide = {7,3}; p.texBottom = {7,3};
    reg.registerBlock(p);
    p.name = "Deepslate Diamond Ore";
    p.texTop = {7,4}; p.texSide = {7,4}; p.texBottom = {7,4};
    reg.registerBlock(p);

    // Cuarzo del Nether
    p.name = "Nether Quartz Ore";
    p.texTop = {8,3}; p.texSide = {8,3}; p.texBottom = {8,3};
    reg.registerBlock(p);

    // Escombros Antiguos (Netherite)
    p.name = "Ancient Debris";
    p.texTop = {9,3}; p.texSide = {9,3}; p.texBottom = {9,3};
    p.hardness = 30.0f; p.blastResistance = 1200.0f;
    reg.registerBlock(p);
}

// ── 2. BLOQUES DE ALMACENAMIENTO / DECORATIVOS ───────────────────────────────
void registerMetalStorageBlocks() {
    auto& reg = BlockRegistry::get();
    BlockProperties p;
    p.solid = true; p.transparent = false; p.gravity = false;
    p.fluid = false; p.opaque = true;
    p.hardness = 5.0f; p.blastResistance = 6.0f;

    p.name = "Coal Block";
    p.texTop = {0,5}; p.texSide = {0,5}; p.texBottom = {0,5};
    reg.registerBlock(p);

    p.name = "Iron Block";
    p.texTop = {1,5}; p.texSide = {1,5}; p.texBottom = {1,5};
    reg.registerBlock(p);

    p.name = "Gold Block";
    p.texTop = {2,5}; p.texSide = {2,5}; p.texBottom = {2,5};
    reg.registerBlock(p);

    p.name = "Diamond Block";
    p.texTop = {3,5}; p.texSide = {3,5}; p.texBottom = {3,5};
    reg.registerBlock(p);

    p.name = "Emerald Block";
    p.texTop = {4,5}; p.texSide = {4,5}; p.texBottom = {4,5};
    reg.registerBlock(p);

    p.name = "Lapis Lazuli Block";
    p.texTop = {5,5}; p.texSide = {5,5}; p.texBottom = {5,5};
    reg.registerBlock(p);

    p.name = "Redstone Block";
    p.texTop = {6,5}; p.texSide = {6,5}; p.texBottom = {6,5};
    reg.registerBlock(p);

    p.name = "Netherite Block";
    p.texTop = {7,5}; p.texSide = {7,5}; p.texBottom = {7,5};
    p.hardness = 50.0f; p.blastResistance = 1200.0f;
    reg.registerBlock(p);

    p.hardness = 5.0f; p.blastResistance = 6.0f;

    p.name = "Raw Iron Block";
    p.texTop = {1,6}; p.texSide = {1,6}; p.texBottom = {1,6};
    reg.registerBlock(p);

    p.name = "Raw Gold Block";
    p.texTop = {2,6}; p.texSide = {2,6}; p.texBottom = {2,6};
    reg.registerBlock(p);

    p.name = "Raw Copper Block";
    p.texTop = {3,6}; p.texSide = {3,6}; p.texBottom = {3,6};
    reg.registerBlock(p);

    // Cuarzo
    p.hardness = 0.8f; p.blastResistance = 0.8f;

    p.name = "Quartz Block";
    p.texTop = {8,5}; p.texSide = {8,5}; p.texBottom = {8,5};
    reg.registerBlock(p);

    p.name = "Smooth Quartz Block";
    p.texTop = {8,6}; p.texSide = {8,6}; p.texBottom = {8,6};
    reg.registerBlock(p);

    p.name = "Quartz Pillar";
    p.texTop = {8,7}; p.texSide = {8,5}; p.texBottom = {8,7};
    reg.registerBlock(p);

    p.name = "Chiseled Quartz Block";
    p.texTop = {8,8}; p.texSide = {8,8}; p.texBottom = {8,8};
    reg.registerBlock(p);

    p.name = "Quartz Bricks";
    p.texTop = {8,9}; p.texSide = {8,9}; p.texBottom = {8,9};
    reg.registerBlock(p);
}

// ── 3. COBRE: OXIDACIONES Y VARIANTES ────────────────────────────────────────
//  4 niveles de oxidación × 2 (normal / cortado) × 2 (encerado / sin encerar)
//  = 16 variantes principales + stairs y slabs de cortado
void registerCopperVariants() {
    auto& reg = BlockRegistry::get();
    BlockProperties p;
    p.solid = true; p.transparent = false; p.gravity = false;
    p.fluid = false; p.opaque = true;
    p.hardness = 3.0f; p.blastResistance = 6.0f;

    // Nivel 0 – sin oxidar
    p.name = "Copper Block";
    p.texTop = {2,7}; p.texSide = {2,7}; p.texBottom = {2,7};
    reg.registerBlock(p);

    p.name = "Cut Copper";
    p.texTop = {2,8}; p.texSide = {2,8}; p.texBottom = {2,8};
    reg.registerBlock(p);
    { auto s = p; s.name = "Cut Copper Stairs"; reg.registerBlock(s); }
    { auto s = p; s.name = "Cut Copper Slab";   reg.registerBlock(s); }

    p.name = "Waxed Copper Block";
    p.texTop = {2,7}; reg.registerBlock(p);
    p.name = "Waxed Cut Copper";
    p.texTop = {2,8}; reg.registerBlock(p);
    { auto s = p; s.name = "Waxed Cut Copper Stairs"; reg.registerBlock(s); }
    { auto s = p; s.name = "Waxed Cut Copper Slab";   reg.registerBlock(s); }

    // Nivel 1 – expuesto
    p.name = "Exposed Copper";
    p.texTop = {2,9}; p.texSide = {2,9}; p.texBottom = {2,9};
    reg.registerBlock(p);
    p.name = "Exposed Cut Copper";
    p.texTop = {2,10}; reg.registerBlock(p);
    { auto s = p; s.name = "Exposed Cut Copper Stairs"; reg.registerBlock(s); }
    { auto s = p; s.name = "Exposed Cut Copper Slab";   reg.registerBlock(s); }

    p.name = "Waxed Exposed Copper";
    p.texTop = {2,9}; reg.registerBlock(p);
    p.name = "Waxed Exposed Cut Copper";
    p.texTop = {2,10}; reg.registerBlock(p);

    // Nivel 2 – envejecido
    p.name = "Weathered Copper";
    p.texTop = {2,11}; p.texSide = {2,11}; p.texBottom = {2,11};
    reg.registerBlock(p);
    p.name = "Weathered Cut Copper";
    p.texTop = {2,12}; reg.registerBlock(p);
    { auto s = p; s.name = "Weathered Cut Copper Stairs"; reg.registerBlock(s); }
    { auto s = p; s.name = "Weathered Cut Copper Slab";   reg.registerBlock(s); }

    p.name = "Waxed Weathered Copper";
    p.texTop = {2,11}; reg.registerBlock(p);
    p.name = "Waxed Weathered Cut Copper";
    p.texTop = {2,12}; reg.registerBlock(p);

    // Nivel 3 – oxidado
    p.name = "Oxidized Copper";
    p.texTop = {2,13}; p.texSide = {2,13}; p.texBottom = {2,13};
    reg.registerBlock(p);
    p.name = "Oxidized Cut Copper";
    p.texTop = {2,14}; reg.registerBlock(p);
    { auto s = p; s.name = "Oxidized Cut Copper Stairs"; reg.registerBlock(s); }
    { auto s = p; s.name = "Oxidized Cut Copper Slab";   reg.registerBlock(s); }

    p.name = "Waxed Oxidized Copper";
    p.texTop = {2,13}; reg.registerBlock(p);
    p.name = "Waxed Oxidized Cut Copper";
    p.texTop = {2,14}; reg.registerBlock(p);

    // Buldge Copper (1.21 - Copper Bulb / Copper Grate / Copper Door / Trapdoor)
    p.name = "Copper Grate";
    p.transparent = true; p.opaque = false;
    p.texTop = {2,15}; p.texSide = {2,15}; p.texBottom = {2,15};
    reg.registerBlock(p);
    p.transparent = false; p.opaque = true;

    p.name = "Copper Door";
    p.texTop = {2,8}; reg.registerBlock(p);
    p.name = "Copper Trapdoor";
    reg.registerBlock(p);
    p.name = "Copper Bulb";
    p.texTop = {3,7}; reg.registerBlock(p);
}
