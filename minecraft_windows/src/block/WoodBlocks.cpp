#include "BlockRegistry.h"
// ─────────────────────────────────────────────────────────────────────────────
//  WoodBlocks.cpp  –  Todos los bloques de madera (vanilla 1.21)
//  Maderas: Oak, Spruce, Birch, Jungle, Acacia, Dark Oak,
//           Mangrove, Cherry, Pale Oak (1.21), Crimson, Warped
// ─────────────────────────────────────────────────────────────────────────────

// Macro para registrar las variantes de puerta/valla/escalera/losa
// de una madera que ya tiene definida su textura de tablón en `p`
#define REGISTER_WOOD_VARIANTS(reg, p)      \
    do {                                     \
        auto _base = p;                      \
        _base.name = p.name + " Door";       \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Trapdoor";   \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Fence";      \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Fence Gate"; \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Stairs";     \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Slab";       \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Button";     \
        reg.registerBlock(_base);            \
        _base.name = p.name + " Pressure Plate"; \
        reg.registerBlock(_base);            \
    } while(0)

void registerWoodBlocks() {
    auto& reg = BlockRegistry::get();
    BlockProperties p;
    p.solid = true; p.transparent = false; p.gravity = false;
    p.fluid = false; p.opaque = true;
    p.hardness = 2.0f; p.blastResistance = 3.0f;

    // ── OAK ──────────────────────────────────────────────────────────────────
    p.name = "Oak Log";
    p.texTop = {5,0}; p.texSide = {6,0}; p.texBottom = {5,0};
    reg.registerBlock(p);

    p.name = "Oak Wood";
    p.texTop = {5,0}; p.texSide = {5,0}; p.texBottom = {5,0};
    reg.registerBlock(p);

    p.name = "Stripped Oak Log";
    p.texTop = {5,1}; p.texSide = {6,1}; p.texBottom = {5,1};
    reg.registerBlock(p);

    p.name = "Stripped Oak Wood";
    p.texTop = {5,1}; p.texSide = {5,1}; p.texBottom = {5,1};
    reg.registerBlock(p);

    p.name = "Oak Planks";
    p.texTop = {2,1}; p.texSide = {2,1}; p.texBottom = {2,1};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── SPRUCE ────────────────────────────────────────────────────────────────
    p.name = "Spruce Log";
    p.texTop = {5,2}; p.texSide = {6,2}; p.texBottom = {5,2};
    reg.registerBlock(p);

    p.name = "Spruce Wood";
    p.texTop = {5,2}; p.texSide = {5,2}; p.texBottom = {5,2};
    reg.registerBlock(p);

    p.name = "Stripped Spruce Log";
    p.texTop = {5,3}; p.texSide = {6,3}; p.texBottom = {5,3};
    reg.registerBlock(p);

    p.name = "Spruce Planks";
    p.texTop = {2,2}; p.texSide = {2,2}; p.texBottom = {2,2};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── BIRCH ─────────────────────────────────────────────────────────────────
    p.name = "Birch Log";
    p.texTop = {5,4}; p.texSide = {6,4}; p.texBottom = {5,4};
    reg.registerBlock(p);

    p.name = "Stripped Birch Log";
    p.texTop = {5,5}; p.texSide = {6,5}; p.texBottom = {5,5};
    reg.registerBlock(p);

    p.name = "Birch Planks";
    p.texTop = {2,3}; p.texSide = {2,3}; p.texBottom = {2,3};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── JUNGLE ────────────────────────────────────────────────────────────────
    p.name = "Jungle Log";
    p.texTop = {5,6}; p.texSide = {6,6}; p.texBottom = {5,6};
    reg.registerBlock(p);

    p.name = "Stripped Jungle Log";
    p.texTop = {5,7}; p.texSide = {6,7}; p.texBottom = {5,7};
    reg.registerBlock(p);

    p.name = "Jungle Planks";
    p.texTop = {2,4}; p.texSide = {2,4}; p.texBottom = {2,4};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── ACACIA ────────────────────────────────────────────────────────────────
    p.name = "Acacia Log";
    p.texTop = {5,8}; p.texSide = {6,8}; p.texBottom = {5,8};
    reg.registerBlock(p);

    p.name = "Stripped Acacia Log";
    p.texTop = {5,9}; p.texSide = {6,9}; p.texBottom = {5,9};
    reg.registerBlock(p);

    p.name = "Acacia Planks";
    p.texTop = {2,5}; p.texSide = {2,5}; p.texBottom = {2,5};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── DARK OAK ──────────────────────────────────────────────────────────────
    p.name = "Dark Oak Log";
    p.texTop = {5,10}; p.texSide = {6,10}; p.texBottom = {5,10};
    reg.registerBlock(p);

    p.name = "Stripped Dark Oak Log";
    p.texTop = {5,11}; p.texSide = {6,11}; p.texBottom = {5,11};
    reg.registerBlock(p);

    p.name = "Dark Oak Planks";
    p.texTop = {2,6}; p.texSide = {2,6}; p.texBottom = {2,6};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── MANGROVE ──────────────────────────────────────────────────────────────
    p.name = "Mangrove Log";
    p.texTop = {5,12}; p.texSide = {6,12}; p.texBottom = {5,12};
    reg.registerBlock(p);

    p.name = "Mangrove Roots";
    p.transparent = true; p.opaque = false;
    p.texTop = {6,13}; p.texSide = {6,13}; p.texBottom = {6,13};
    reg.registerBlock(p);
    p.transparent = false; p.opaque = true;

    p.name = "Mangrove Planks";
    p.texTop = {2,7}; p.texSide = {2,7}; p.texBottom = {2,7};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── CHERRY ────────────────────────────────────────────────────────────────
    p.name = "Cherry Log";
    p.texTop = {5,13}; p.texSide = {6,13}; p.texBottom = {5,13};
    reg.registerBlock(p);

    p.name = "Stripped Cherry Log";
    p.texTop = {5,14}; p.texSide = {6,14}; p.texBottom = {5,14};
    reg.registerBlock(p);

    p.name = "Cherry Planks";
    p.texTop = {2,8}; p.texSide = {2,8}; p.texBottom = {2,8};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── PALE OAK (1.21) ───────────────────────────────────────────────────────
    p.name = "Pale Oak Log";
    p.texTop = {5,15}; p.texSide = {6,15}; p.texBottom = {5,15};
    reg.registerBlock(p);

    p.name = "Pale Oak Planks";
    p.texTop = {2,9}; p.texSide = {2,9}; p.texBottom = {2,9};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── CRIMSON (Nether) ──────────────────────────────────────────────────────
    p.name = "Crimson Stem";
    p.texTop = {7,0}; p.texSide = {7,1}; p.texBottom = {7,0};
    reg.registerBlock(p);

    p.name = "Stripped Crimson Stem";
    p.texTop = {7,2}; p.texSide = {7,3}; p.texBottom = {7,2};
    reg.registerBlock(p);

    p.name = "Crimson Planks";
    p.texTop = {2,10}; p.texSide = {2,10}; p.texBottom = {2,10};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);

    // ── WARPED (Nether) ───────────────────────────────────────────────────────
    p.name = "Warped Stem";
    p.texTop = {7,4}; p.texSide = {7,5}; p.texBottom = {7,4};
    reg.registerBlock(p);

    p.name = "Stripped Warped Stem";
    p.texTop = {7,6}; p.texSide = {7,7}; p.texBottom = {7,6};
    reg.registerBlock(p);

    p.name = "Warped Planks";
    p.texTop = {2,11}; p.texSide = {2,11}; p.texBottom = {2,11};
    reg.registerBlock(p);
    REGISTER_WOOD_VARIANTS(reg, p);
}
