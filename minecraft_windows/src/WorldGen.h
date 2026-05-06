#pragma once
#include "Chunk.h"
#include "PerlinNoise.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

class WorldGen {
    PerlinNoise terrain;    // heightmap
    PerlinNoise caves;      // 3-D cave tunnels
    PerlinNoise detail;     // surface detail / biome variation
    PerlinNoise treeMap;    // sparse tree placement

public:
    explicit WorldGen(unsigned int seed = 42)
        : terrain(seed),
          caves(seed + 7919u),
          detail(seed + 104729u),
          treeMap(seed + 15731u) {}

    void generate(Chunk& chunk) const {
        const int WX = chunk.cx * CHUNK_W;
        const int WZ = chunk.cz * CHUNK_D;

        // ── 1. Build heightmap ────────────────────────────────────────────────
        int hmap[CHUNK_W][CHUNK_D];
        float moisture[CHUNK_W][CHUNK_D];

        for (int x = 0; x < CHUNK_W; x++) {
            for (int z = 0; z < CHUNK_D; z++) {
                double nx = (WX + x) / 180.0;
                double nz = (WZ + z) / 180.0;

                // Continent shape  (large, low-frequency)
                double continent = terrain.octave(nx * 0.3, nz * 0.3, 3, 0.5, 2.0);
                continent = (continent + 1.0) * 0.5;  // [0,1]

                // Mountains (mid-frequency)
                double mountain = terrain.octave(nx * 0.8, nz * 0.8, 5, 0.45, 2.2);
                mountain = std::max(0.0, mountain);
                mountain = mountain * mountain;        // sharp peaks

                // Fine erosion
                double erosion = detail.octave(nx * 2.0, nz * 2.0, 3, 0.5, 2.0) * 0.1;

                double h = continent * 0.5 + mountain * 0.4 + erosion;
                hmap[x][z] = (int)(SEA_LEVEL + h * 60.0 - 8.0);
                hmap[x][z] = std::clamp(hmap[x][z], 1, CHUNK_H - 32);

                // Moisture for biome detection
                double m = detail.octave(nx * 0.4 + 100, nz * 0.4 + 100, 3, 0.5, 2.0);
                moisture[x][z] = (float)((m + 1.0) * 0.5);
            }
        }

        // ── 2. Fill blocks ────────────────────────────────────────────────────
        for (int x = 0; x < CHUNK_W; x++) {
        for (int z = 0; z < CHUNK_D; z++) {
            int top = hmap[x][z];
            bool sandy = moisture[x][z] < 0.35f || top <= SEA_LEVEL + 2;

            // Bedrock floor
            chunk.set(x, 0, z, BlockType::BEDROCK);
            for (int y = 1; y < 5; y++) {
                // Random bedrock ceiling (y=1 always bedrock, y=2..4 sometimes)
                if ((int)(terrain.noise(WX+x, y, WZ+z) * 1000) % (y+1) == 0)
                    chunk.set(x, y, z, BlockType::BEDROCK);
                else
                    chunk.set(x, y, z, BlockType::STONE);
            }

            for (int y = 5; y < CHUNK_H; y++) {
                if (y > top) {
                    // Air or water
                    if (y <= SEA_LEVEL)
                        chunk.set(x, y, z, BlockType::WATER);
                    // else AIR (already 0)
                    continue;
                }

                // Cave carving (3-D Perlin threshold)
                if (y > 5 && y < top - 2) {
                    double cx2 = (WX + x) / 28.0;
                    double cy2 = y          / 18.0;
                    double cz2 = (WZ + z)  / 28.0;
                    // Two-pass: AND of two cave noises → more natural tunnels
                    double c1 = caves.noise(cx2, cy2, cz2);
                    double c2 = caves.noise(cx2 * 0.5 + 50, cy2 * 0.5, cz2 * 0.5 + 50);
                    if (std::abs(c1) < 0.08 && std::abs(c2) < 0.12) continue; // hollow
                }

                // Surface blocks
                if (y == top) {
                    chunk.set(x, y, z, sandy ? BlockType::SAND : BlockType::GRASS);
                } else if (y >= top - 3) {
                    chunk.set(x, y, z, sandy ? BlockType::SAND : BlockType::DIRT);
                } else {
                    chunk.set(x, y, z, BlockType::STONE);
                }
            }
        }
        }

        // ── 3. Tree placement ─────────────────────────────────────────────────
        // Keep trees 2 blocks from chunk edge to avoid cross-chunk artifacts
        for (int x = 2; x < CHUNK_W - 2; x++) {
        for (int z = 2; z < CHUNK_D - 2; z++) {
            int top = hmap[x][z];
            if (top <= SEA_LEVEL + 3)  continue; // no trees in water/sand coast
            if (moisture[x][z] < 0.35f) continue; // desert biome

            double tn = treeMap.noise((WX + x) / 7.0, 0.0, (WZ + z) / 7.0);
            if (tn > 0.72) {
                placeTree(chunk, x, top + 1, z);
            }
        }
        }
    }

private:
    // ── Oak tree ──────────────────────────────────────────────────────────────
    void placeTree(Chunk& chunk, int bx, int baseY, int bz) const {
        const int trunkH = 4 + (int)(std::abs(treeMap.noise(bx, baseY, bz)) * 3);

        // Trunk
        for (int y = baseY; y < baseY + trunkH && y < CHUNK_H; y++)
            chunk.set(bx, y, bz, BlockType::WOOD);

        // Leaf layers (from trunkH-2 upward)
        int leafBase = baseY + trunkH - 2;
        for (int dy = 0; dy <= 3 && leafBase + dy < CHUNK_H; dy++) {
            int ly = leafBase + dy;
            int radius = (dy <= 1) ? 2 : 1;
            for (int dx = -radius; dx <= radius; dx++) {
            for (int dz = -radius; dz <= radius; dz++) {
                if (std::abs(dx) == radius && std::abs(dz) == radius) continue; // round corners
                int lx = bx + dx, lz = bz + dz;
                if (lx < 0 || lx >= CHUNK_W || lz < 0 || lz >= CHUNK_D) continue;
                if (chunk.get(lx, ly, lz) == BlockType::AIR)
                    chunk.set(lx, ly, lz, BlockType::LEAVES);
            }
            }
        }
        // Top single leaf
        if (baseY + trunkH < CHUNK_H)
            chunk.set(bx, baseY + trunkH, bz, BlockType::LEAVES);
    }
};
