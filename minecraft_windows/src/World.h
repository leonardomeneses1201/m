#pragma once
#include "Chunk.h"
#include "WorldGen.h"
#include <unordered_map>
#include <memory>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

// ── Hash map key for chunk coordinates ────────────────────────────────────────
struct ChunkKey {
    int cx, cz;
    bool operator==(const ChunkKey& o) const { return cx == o.cx && cz == o.cz; }
};
struct ChunkKeyHash {
    size_t operator()(const ChunkKey& k) const {
        // Cantor-like pairing that handles negative coordinates
        size_t h1 = std::hash<int>{}(k.cx);
        size_t h2 = std::hash<int>{}(k.cz);
        return h1 ^ (h2 * 2654435761u + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
    }
};

using ChunkMap = std::unordered_map<ChunkKey, std::unique_ptr<Chunk>, ChunkKeyHash>;

class World {
public:
    int renderDist = 8;     // chunks in each direction
    WorldGen gen;
    ChunkMap chunks;

    explicit World(unsigned int seed = 42) : gen(seed) {}

    // ── Chunk access ───────────────────────────────────────────────────────────
    Chunk* getChunk(int cx, int cz) const {
        auto it = chunks.find({ cx, cz });
        return it != chunks.end() ? it->second.get() : nullptr;
    }

    Chunk* loadChunk(int cx, int cz) {
        auto& ptr = chunks[{ cx, cz }];
        if (!ptr) {
            ptr = std::make_unique<Chunk>(cx, cz);
            gen.generate(*ptr);
        }
        return ptr.get();
    }

    // ── Per-frame update: load/mesh/unload ────────────────────────────────────
    void update(int pcx, int pcz) {
        // 1. Ensure chunks in range exist
        for (int dx = -renderDist; dx <= renderDist; dx++)
            for (int dz = -renderDist; dz <= renderDist; dz++)
                loadChunk(pcx + dx, pcz + dz);

        // 2. Rebuild meshes that are dirty (limit per frame to avoid stalls)
        int rebuilt = 0;
        for (auto& [key, chunk] : chunks) {
            if (!chunk->dirty) continue;
            if (rebuilt >= 4) break;   // max 4 chunk rebuilds per frame
            chunk->buildMesh(
                getChunk(key.cx - 1, key.cz),
                getChunk(key.cx + 1, key.cz),
                getChunk(key.cx, key.cz - 1),
                getChunk(key.cx, key.cz + 1)
            );
            ++rebuilt;
        }

        // 3. Evict chunks that are too far away
        for (auto it = chunks.begin(); it != chunks.end(); ) {
            int d = std::max(std::abs(it->first.cx - pcx),
                             std::abs(it->first.cz - pcz));
            if (d > renderDist + 2)
                it = chunks.erase(it);
            else
                ++it;
        }
    }

    // ── Render all loaded chunks within view distance ─────────────────────────
    void render(int pcx, int pcz) const {
        for (auto& [key, chunk] : chunks) {
            int d = std::max(std::abs(key.cx - pcx),
                             std::abs(key.cz - pcz));
            if (d <= renderDist)
                chunk->render();
        }
    }

    // ── Block access by world coordinates ─────────────────────────────────────
    BlockType getBlock(int wx, int wy, int wz) const {
        if (wy < 0 || wy >= CHUNK_H) return BlockType::AIR;
        int cx = (int)std::floor(wx / (float)CHUNK_W);
        int cz = (int)std::floor(wz / (float)CHUNK_D);
        Chunk* c = getChunk(cx, cz);
        if (!c) return BlockType::AIR;
        int bx = wx - cx * CHUNK_W;
        int bz = wz - cz * CHUNK_D;
        return c->get(bx, wy, bz);
    }

    void setBlock(int wx, int wy, int wz, BlockType t) {
        if (wy < 0 || wy >= CHUNK_H) return;
        int cx = (int)std::floor(wx / (float)CHUNK_W);
        int cz = (int)std::floor(wz / (float)CHUNK_D);
        int bx = wx - cx * CHUNK_W;
        int bz = wz - cz * CHUNK_D;
        Chunk* c = getChunk(cx, cz);
        if (!c) return;
        c->set(bx, wy, bz, t);
        c->dirty = true;
        // Mark edge neighbors dirty too
        if (bx == 0         && getChunk(cx-1, cz)) getChunk(cx-1, cz)->dirty = true;
        if (bx == CHUNK_W-1 && getChunk(cx+1, cz)) getChunk(cx+1, cz)->dirty = true;
        if (bz == 0         && getChunk(cx, cz-1)) getChunk(cx, cz-1)->dirty = true;
        if (bz == CHUNK_D-1 && getChunk(cx, cz+1)) getChunk(cx, cz+1)->dirty = true;
    }

    // ── DDA voxel raycast ─────────────────────────────────────────────────────
    // Returns true if a solid (non-air, non-water) block was hit.
    // hitBlock  = integer coords of the hit block
    // hitNormal = face normal (one of ±X, ±Y, ±Z unit vectors)
    bool raycast(glm::vec3 origin, glm::vec3 dir, float maxDist,
                 glm::ivec3& hitBlock, glm::ivec3& hitNormal) const
    {
        const float EPS = 1e-6f;
        glm::vec3  pos  = origin;
        glm::ivec3 cell = { (int)std::floor(pos.x),
                            (int)std::floor(pos.y),
                            (int)std::floor(pos.z) };
        glm::ivec3 step = { dir.x > 0 ? 1 : -1,
                            dir.y > 0 ? 1 : -1,
                            dir.z > 0 ? 1 : -1 };

        // Distance along ray between integer boundaries
        glm::vec3 tDelta = { 1.f / (std::abs(dir.x) + EPS),
                             1.f / (std::abs(dir.y) + EPS),
                             1.f / (std::abs(dir.z) + EPS) };

        // Distance to first boundary in each axis
        auto frac = [](float v, int s) -> float {
            return s > 0 ? (std::ceil(v) - v) : (v - std::floor(v));
        };
        glm::vec3 tMax = { frac(pos.x, step.x) * tDelta.x,
                           frac(pos.y, step.y) * tDelta.y,
                           frac(pos.z, step.z) * tDelta.z };

        glm::ivec3 lastStep = { 0, 0, 0 };
        float t = 0.f;

        while (t < maxDist) {
            // Advance to next voxel boundary
            if (tMax.x < tMax.y && tMax.x < tMax.z) {
                t = tMax.x; tMax.x += tDelta.x; cell.x += step.x; lastStep = {step.x, 0, 0};
            } else if (tMax.y < tMax.z) {
                t = tMax.y; tMax.y += tDelta.y; cell.y += step.y; lastStep = {0, step.y, 0};
            } else {
                t = tMax.z; tMax.z += tDelta.z; cell.z += step.z; lastStep = {0, 0, step.z};
            }
            if (t >= maxDist) break;

            BlockType bt = getBlock(cell.x, cell.y, cell.z);
            if (bt != BlockType::AIR && bt != BlockType::WATER) {
                hitBlock  = cell;
                hitNormal = -lastStep;
                return true;
            }
        }
        return false;
    }

    // Find first solid block at or above y for spawn
    int findSpawnY(int wx, int wz) const {
        for (int y = CHUNK_H - 1; y >= 0; y--) {
            BlockType bt = getBlock(wx, y, wz);
            if (isSolid(bt)) return y + 2;
        }
        return SEA_LEVEL + 5;
    }
};
