#pragma once
// ─────────────────────────────────────────────────────────────────────────────
//  Block.h  –  Propiedades de bloque y clase base
// ─────────────────────────────────────────────────────────────────────────────
#include <string>
#include <glm/glm.hpp>
#include <cstdint>

struct BlockProperties {
    std::string name;
    glm::vec2 texTop    = {0, 0};
    glm::vec2 texSide   = {0, 0};
    glm::vec2 texBottom = {0, 0};
    bool  solid          = true;
    bool  transparent    = false;
    float hardness       = 1.0f;    // segundos para romper a mano (-1 = irrompible)
    float blastResistance= 1.0f;    // resistencia a explosiones
    bool  gravity        = false;   // cae como arena/grava
    bool  fluid          = false;
    bool  opaque         = true;    // bloquea la luz
};

using BlockID = uint16_t;
constexpr BlockID AIR_ID = 0;

class Block {
public:
    Block(BlockID id, const BlockProperties& props) : m_id(id), m_props(props) {}

    BlockID              getId()    const { return m_id; }
    const std::string&   getName()  const { return m_props.name; }
    const BlockProperties& getProps() const { return m_props; }

    bool  isSolid()         const { return m_props.solid; }
    bool  isTransparent()   const { return m_props.transparent; }
    bool  isFluid()         const { return m_props.fluid; }
    bool  isOpaque()        const { return m_props.opaque; }
    float getHardness()     const { return m_props.hardness; }
    float getBlastResistance() const { return m_props.blastResistance; }
    bool  hasGravity()      const { return m_props.gravity; }

    // face: 0=bottom, 1=top, 2..5=sides
    glm::vec2 getUV(int face) const {
        switch (face) {
            case 0:  return m_props.texBottom;
            case 1:  return m_props.texTop;
            default: return m_props.texSide;
        }
    }

private:
    BlockID         m_id;
    BlockProperties m_props;
};
