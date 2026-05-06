#include "Chunk.h"
#include <cstring>

// Face directions: 0=-Y(bottom), 1=+Y(top), 2=-Z(north), 3=+Z(south), 4=-X(west), 5=+X(east)
static const float FACE_NORMALS[6][3] = {
    { 0,-1, 0},{0,1,0},{0,0,-1},{0,0,1},{-1,0,0},{1,0,0}
};
// Face brightness
static const float FACE_LIGHT[6] = {0.4f, 1.0f, 0.6f, 0.6f, 0.8f, 0.8f};

// Quad vertices for each face (relative to block origin)
// Each face: 4 vertices, each vertex = {dx,dy,dz, du,dv}
static const float FACE_VERTS[6][4][5] = {
    // Bottom (-Y)
    {{0,0,0,0,1},{1,0,0,1,1},{1,0,1,1,0},{0,0,1,0,0}},
    // Top (+Y)
    {{0,1,0,0,0},{0,1,1,0,1},{1,1,1,1,1},{1,1,0,1,0}},
    // North (-Z)
    {{0,0,0,0,1},{0,1,0,0,0},{1,1,0,1,0},{1,0,0,1,1}},
    // South (+Z)
    {{1,0,1,0,1},{1,1,1,0,0},{0,1,1,1,0},{0,0,1,1,1}},
    // West (-X)
    {{0,0,1,0,1},{0,1,1,0,0},{0,1,0,1,0},{0,0,0,1,1}},
    // East (+X)
    {{1,0,0,0,1},{1,1,0,0,0},{1,1,1,1,0},{1,0,1,1,1}},
};

// Atlas tile size (assumes 256x256 atlas with 16x16 px tiles → 16 tiles per row)
static const float ATLAS_TILE = 1.0f / 16.0f;

Chunk::Chunk(int cx, int cz) : cx(cx), cz(cz) {
    memset(blocks, 0, sizeof(blocks));
}

Chunk::~Chunk() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

BlockType Chunk::get(int x, int y, int z) const {
    if (x<0||x>=CHUNK_W||y<0||y>=CHUNK_H||z<0||z>=CHUNK_D) return BlockType::AIR;
    return blocks[x][y][z];
}

void Chunk::set(int x, int y, int z, BlockType t) {
    if (x<0||x>=CHUNK_W||y<0||y>=CHUNK_H||z<0||z>=CHUNK_D) return;
    blocks[x][y][z] = t;
    dirty = true;
}

BlockType Chunk::getWithNeighbors(int x, int y, int z,
                                   const Chunk* nx, const Chunk* px,
                                   const Chunk* nz, const Chunk* pz) const {
    if (y < 0 || y >= CHUNK_H) return BlockType::AIR;
    if (x < 0)       return nx ? nx->get(CHUNK_W+x, y, z) : BlockType::STONE;
    if (x >= CHUNK_W) return px ? px->get(x-CHUNK_W, y, z) : BlockType::STONE;
    if (z < 0)       return nz ? nz->get(x, y, CHUNK_D+z) : BlockType::STONE;
    if (z >= CHUNK_D) return pz ? pz->get(x, y, z-CHUNK_D) : BlockType::STONE;
    return blocks[x][y][z];
}

void Chunk::addFace(std::vector<Vertex>& verts, std::vector<uint32_t>& idx,
                    int wx, int wy, int wz, int face, BlockType bt) {
    const BlockData& bd = BLOCK_DATA[(int)bt];
    const float* uv;
    if (face == 1) uv = bd.texTop;
    else if (face == 0) uv = bd.texBottom;
    else uv = bd.texSide;

    float u0 = uv[0] * ATLAS_TILE;
    float v0 = uv[1] * ATLAS_TILE;
    float light = FACE_LIGHT[face];

    uint32_t base = (uint32_t)verts.size();
    for (int i = 0; i < 4; i++) {
        const float* fv = FACE_VERTS[face][i];
        Vertex v;
        v.x = wx + fv[0];
        v.y = wy + fv[1];
        v.z = wz + fv[2];
        v.u = u0 + fv[3] * ATLAS_TILE;
        v.v = v0 + fv[4] * ATLAS_TILE;
        v.light = light;
        verts.push_back(v);
    }
    // Two triangles: 0,1,2  0,2,3
    idx.push_back(base+0); idx.push_back(base+1); idx.push_back(base+2);
    idx.push_back(base+0); idx.push_back(base+2); idx.push_back(base+3);
}

void Chunk::buildMesh(const Chunk* nx, const Chunk* px,
                      const Chunk* nz, const Chunk* pz) {
    std::vector<Vertex> verts;
    std::vector<uint32_t> idx;
    verts.reserve(4096);
    idx.reserve(6144);

    // Neighbor offsets for each face
    static const int NOFF[6][3] = {
        {0,-1,0},{0,1,0},{0,0,-1},{0,0,1},{-1,0,0},{1,0,0}
    };

    int worldX = cx * CHUNK_W;
    int worldZ = cz * CHUNK_D;

    for (int x = 0; x < CHUNK_W; x++)
    for (int y = 0; y < CHUNK_H; y++)
    for (int z = 0; z < CHUNK_D; z++) {
        BlockType bt = blocks[x][y][z];
        if (bt == BlockType::AIR) continue;

        for (int f = 0; f < 6; f++) {
            int nx2 = x+NOFF[f][0];
            int ny2 = y+NOFF[f][1];
            int nz2 = z+NOFF[f][2];
            BlockType neighbor = getWithNeighbors(nx2, ny2, nz2, nx, px, nz, pz);
            if (isTransparent(neighbor)) {
                addFace(verts, idx, worldX+x, y, worldZ+z, f, bt);
            }
        }
    }

    indexCount = (int)idx.size();
    uploadMesh(verts, idx);
    dirty = false;
}

void Chunk::uploadMesh(const std::vector<Vertex>& verts,
                       const std::vector<uint32_t>& indices) {
    if (!vao) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(Vertex), verts.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // Light
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Chunk::render() const {
    if (indexCount == 0 || !vao) return;
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
