#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  BlockRegistry.h  –  Registro dinámico de bloques (singleton)
//  Uso: BlockRegistry::get().registerBlock(props)
//       BlockRegistry::get().getBlock(id)
// ─────────────────────────────────────────────────────────────────────────────
#include "Block.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>

class BlockRegistry {
public:
    static BlockRegistry& get() {
        static BlockRegistry instance;
        return instance;
    }

    // Registra un bloque y devuelve su ID (1, 2, 3 ...)
    BlockID registerBlock(const BlockProperties& props) {
        BlockID id = static_cast<BlockID>(m_blocks.size() + 1);
        auto blk = std::make_unique<Block>(id, props);
        m_nameToId[props.name] = id;
        m_idToPtr.push_back(blk.get());
        m_blocks.push_back(std::move(blk));
        return id;
    }

    // Acceso por ID  (devuelve nullptr si no existe)
    const Block* getBlock(BlockID id) const {
        if (id == AIR_ID || id > static_cast<BlockID>(m_idToPtr.size()))
            return nullptr;
        return m_idToPtr[id - 1];
    }

    // Acceso por nombre
    const Block* getBlockByName(const std::string& name) const {
        auto it = m_nameToId.find(name);
        return (it != m_nameToId.end()) ? getBlock(it->second) : nullptr;
    }

    BlockID getBlockId(const std::string& name) const {
        auto* b = getBlockByName(name);
        return b ? b->getId() : AIR_ID;
    }

    // Total de bloques registrados (sin contar el aire)
    size_t count() const { return m_blocks.size(); }

    const std::vector<const Block*>& allBlocks() const { return m_idToPtr; }

private:
    BlockRegistry() = default;
    BlockRegistry(const BlockRegistry&) = delete;
    BlockRegistry& operator=(const BlockRegistry&) = delete;

    std::vector<std::unique_ptr<Block>> m_blocks;
    std::vector<const Block*>           m_idToPtr;
    std::unordered_map<std::string, BlockID> m_nameToId;
};

// ── Helpers globales ──────────────────────────────────────────────────────────
inline const Block* getBlock(BlockID id) {
    return BlockRegistry::get().getBlock(id);
}
inline BlockID getBlockId(const std::string& name) {
    return BlockRegistry::get().getBlockId(name);
}
inline bool isSolid(BlockID id) {
    auto* b = getBlock(id);
    return b && b->isSolid();
}
inline bool isTransparent(BlockID id) {
    auto* b = getBlock(id);
    return !b || b->isTransparent();
}
