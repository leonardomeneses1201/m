#pragma once
#include "blocks/Block.h"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

constexpr int CHUNK_W = 16;
constexpr int CHUNK_H = 256;
constexpr int CHUNK_D = 16;
constexpr int SEA_LEVEL = 64;

class Chunk {
public:
    int cx, cz; // chunk coords (world / CHUNK_W)
    BlockType blocks[CHUNK_W][CHUNK_H][CHUNK_D];

    // Render mesh
    GLuint vao = 0, vbo = 0, ebo = 0;
    int indexCount = 0;
    bool dirty = true; // needs mesh rebuild

    Chunk(int cx, int cz);
    ~Chunk();

    BlockType get(int x, int y, int z) const;
    void set(int x, int y, int z, BlockType t);

    // Build the opaque mesh. Needs neighbor chunks for face culling at borders.
    void buildMesh(const Chunk* nx, const Chunk* px,
                   const Chunk* nz, const Chunk* pz);

    void render() const;

private:
    struct Vertex {
        float x, y, z;
        float u, v;
        float light; // simple AO / face lighting
    };

    void uploadMesh(const std::vector<Vertex>& verts,
                    const std::vector<uint32_t>& indices);

    void addFace(std::vector<Vertex>& verts, std::vector<uint32_t>& idx,
                 int wx, int wy, int wz, int face, BlockType bt);

    BlockType getWithNeighbors(int x, int y, int z,
                               const Chunk* nx, const Chunk* px,
                               const Chunk* nz, const Chunk* pz) const;
};
