#pragma once
#include <cstdint>

enum class BlockType : uint8_t {
    AIR = 0,
    GRASS,
    DIRT,
    STONE,
    SAND,
    WOOD,
    LEAVES,
    WATER,
    BEDROCK,
    COUNT
};

struct BlockData {
    const char* name;
    // UV offsets in texture atlas (16x16 tiles, 16px each = 256x256 atlas)
    // [top_u, top_v, side_u, side_v, bottom_u, bottom_v]
    float texTop[2];
    float texSide[2];
    float texBottom[2];
    bool solid;
    bool transparent;
};

inline BlockData BLOCK_DATA[] = {
    // AIR
    {"Air", {0,0},{0,0},{0,0}, false, true},
    // GRASS
    {"Grass", {0,0},{1,0},{2,0}, true, false},
    // DIRT
    {"Dirt", {2,0},{2,0},{2,0}, true, false},
    // STONE
    {"Stone", {3,0},{3,0},{3,0}, true, false},
    // SAND
    {"Sand", {4,0},{4,0},{4,0}, true, false},
    // WOOD
    {"Wood", {5,0},{6,0},{5,0}, true, false},
    // LEAVES
    {"Leaves", {7,0},{7,0},{7,0}, true, true},
    // WATER
    {"Water", {8,0},{8,0},{8,0}, false, true},
    // BEDROCK
    {"Bedrock", {9,0},{9,0},{9,0}, true, false},
};

inline bool isTransparent(BlockType t) {
    return BLOCK_DATA[(int)t].transparent;
}
inline bool isSolid(BlockType t) {
    return BLOCK_DATA[(int)t].solid;
}
