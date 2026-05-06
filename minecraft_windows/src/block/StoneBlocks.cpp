#include "BlockRegistry.h"
// ─────────────────────────────────────────────────────────────────────────────
//  StoneBlocks.cpp  –  Todos los bloques de piedra (vanilla 1.21)
// ─────────────────────────────────────────────────────────────────────────────

#define REGISTER_STONE_VARIANTS(reg, p)          \
    do {                                          \
        auto _b = p;                              \
        _b.name = p.name + " Stairs";             \
        reg.registerBlock(_b);                    \
        _b.name = p.name + " Slab";               \
        reg.registerBlock(_b);                    \
        _b.name = p.name + " Wall";               \
        reg.registerBlock(_b);                    \
    } while(0)

void registerStoneBlocks() {
    auto& reg = BlockRegistry::get();
    BlockProperties p;
    p.solid = true; p.transparent = false; p.gravity = false;
    p.fluid = false; p.opaque = true;

    // ── PIEDRA BÁSICA ─────────────────────────────────────────────────────────
    p.name = "Stone";
    p.texTop = {3,0}; p.texSide = {3,0}; p.texBottom = {3,0};
    p.hardness = 1.5f; p.blastResistance = 6.0f;
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Cobblestone";
    p.texTop = {3,1}; p.texSide = {3,1}; p.texBottom = {3,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Mossy Cobblestone";
    p.texTop = {3,2}; p.texSide = {3,2}; p.texBottom = {3,2};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Smooth Stone";
    p.texTop = {3,3}; p.texSide = {3,3}; p.texBottom = {3,3};
    reg.registerBlock(p);
    { auto s = p; s.name = "Smooth Stone Slab"; reg.registerBlock(s); }

    p.name = "Stone Bricks";
    p.texTop = {3,4}; p.texSide = {3,4}; p.texBottom = {3,4};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Cracked Stone Bricks";
    p.texTop = {3,5}; p.texSide = {3,5}; p.texBottom = {3,5};
    reg.registerBlock(p);

    p.name = "Mossy Stone Bricks";
    p.texTop = {3,6}; p.texSide = {3,6}; p.texBottom = {3,6};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Chiseled Stone Bricks";
    p.texTop = {3,7}; p.texSide = {3,7}; p.texBottom = {3,7};
    reg.registerBlock(p);

    // ── GRANITO ───────────────────────────────────────────────────────────────
    p.name = "Granite";
    p.texTop = {4,0}; p.texSide = {4,0}; p.texBottom = {4,0};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Granite";
    p.texTop = {4,1}; p.texSide = {4,1}; p.texBottom = {4,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    // ── DIORITA ───────────────────────────────────────────────────────────────
    p.name = "Diorite";
    p.texTop = {4,2}; p.texSide = {4,2}; p.texBottom = {4,2};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Diorite";
    p.texTop = {4,3}; p.texSide = {4,3}; p.texBottom = {4,3};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    // ── ANDESITA ──────────────────────────────────────────────────────────────
    p.name = "Andesite";
    p.texTop = {4,4}; p.texSide = {4,4}; p.texBottom = {4,4};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Andesite";
    p.texTop = {4,5}; p.texSide = {4,5}; p.texBottom = {4,5};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    // ── DEEPSLATE ─────────────────────────────────────────────────────────────
    p.hardness = 3.0f; p.blastResistance = 6.0f;

    p.name = "Deepslate";
    p.texTop = {7,0}; p.texSide = {7,0}; p.texBottom = {7,0};
    reg.registerBlock(p);

    p.name = "Cobbled Deepslate";
    p.texTop = {7,1}; p.texSide = {7,1}; p.texBottom = {7,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Deepslate";
    p.texTop = {7,2}; p.texSide = {7,2}; p.texBottom = {7,2};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Deepslate Bricks";
    p.texTop = {7,3}; p.texSide = {7,3}; p.texBottom = {7,3};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Cracked Deepslate Bricks";
    p.texTop = {7,4}; p.texSide = {7,4}; p.texBottom = {7,4};
    reg.registerBlock(p);

    p.name = "Deepslate Tiles";
    p.texTop = {7,5}; p.texSide = {7,5}; p.texBottom = {7,5};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Chiseled Deepslate";
    p.texTop = {7,6}; p.texSide = {7,6}; p.texBottom = {7,6};
    reg.registerBlock(p);

    // ── TUFF ──────────────────────────────────────────────────────────────────
    p.hardness = 1.5f; p.blastResistance = 6.0f;

    p.name = "Tuff";
    p.texTop = {8,0}; p.texSide = {8,0}; p.texBottom = {8,0};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Tuff";
    p.texTop = {8,1}; p.texSide = {8,1}; p.texBottom = {8,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Tuff Bricks";
    p.texTop = {8,2}; p.texSide = {8,2}; p.texBottom = {8,2};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Chiseled Tuff";
    p.texTop = {8,3}; p.texSide = {8,3}; p.texBottom = {8,3};
    reg.registerBlock(p);

    // ── BLACKSTONE ────────────────────────────────────────────────────────────
    p.name = "Blackstone";
    p.texTop = {9,0}; p.texSide = {9,0}; p.texBottom = {9,0};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Blackstone";
    p.texTop = {9,1}; p.texSide = {9,1}; p.texBottom = {9,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Polished Blackstone Bricks";
    p.texTop = {9,2}; p.texSide = {9,2}; p.texBottom = {9,2};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Chiseled Polished Blackstone";
    p.texTop = {9,3}; p.texSide = {9,3}; p.texBottom = {9,3};
    reg.registerBlock(p);

    p.name = "Gilded Blackstone";
    p.texTop = {9,4}; p.texSide = {9,4}; p.texBottom = {9,4};
    reg.registerBlock(p);

    // ── BASALTO ───────────────────────────────────────────────────────────────
    p.name = "Basalt";
    p.texTop = {10,0}; p.texSide = {10,1}; p.texBottom = {10,0};
    p.hardness = 1.25f; p.blastResistance = 4.2f;
    reg.registerBlock(p);

    p.name = "Smooth Basalt";
    p.texTop = {10,2}; p.texSide = {10,2}; p.texBottom = {10,2};
    reg.registerBlock(p);

    p.name = "Polished Basalt";
    p.texTop = {10,3}; p.texSide = {10,4}; p.texBottom = {10,3};
    reg.registerBlock(p);

    // ── NETHER ────────────────────────────────────────────────────────────────
    p.hardness = 0.4f; p.blastResistance = 0.4f;

    p.name = "Netherrack";
    p.texTop = {11,0}; p.texSide = {11,0}; p.texBottom = {11,0};
    reg.registerBlock(p);

    p.hardness = 2.0f; p.blastResistance = 6.0f;

    p.name = "Nether Bricks";
    p.texTop = {11,1}; p.texSide = {11,1}; p.texBottom = {11,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Cracked Nether Bricks";
    p.texTop = {11,2}; p.texSide = {11,2}; p.texBottom = {11,2};
    reg.registerBlock(p);

    p.name = "Chiseled Nether Bricks";
    p.texTop = {11,3}; p.texSide = {11,3}; p.texBottom = {11,3};
    reg.registerBlock(p);

    p.name = "Red Nether Bricks";
    p.texTop = {11,4}; p.texSide = {11,4}; p.texBottom = {11,4};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "Soul Sand";
    p.texTop = {11,5}; p.texSide = {11,5}; p.texBottom = {11,5};
    p.hardness = 0.5f;
    reg.registerBlock(p);

    p.name = "Soul Soil";
    p.texTop = {11,6}; p.texSide = {11,6}; p.texBottom = {11,6};
    reg.registerBlock(p);

    p.name = "Magma Block";
    p.texTop = {11,7}; p.texSide = {11,7}; p.texBottom = {11,7};
    p.hardness = 0.5f;
    reg.registerBlock(p);

    // ── END ───────────────────────────────────────────────────────────────────
    p.hardness = 3.0f; p.blastResistance = 9.0f;

    p.name = "End Stone";
    p.texTop = {12,0}; p.texSide = {12,0}; p.texBottom = {12,0};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    p.name = "End Stone Bricks";
    p.texTop = {12,1}; p.texSide = {12,1}; p.texBottom = {12,1};
    reg.registerBlock(p);
    REGISTER_STONE_VARIANTS(reg, p);

    // ── BEDROCK ───────────────────────────────────────────────────────────────
    p.name = "Bedrock";
    p.texTop = {13,0}; p.texSide = {13,0}; p.texBottom = {13,0};
    p.hardness = -1.0f;                 // irrompible
    p.blastResistance = 3600000.0f;
    reg.registerBlock(p);
}
