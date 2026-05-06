# 🧱 block/

Sistema de registro dinámico de bloques. Reemplaza al antiguo `Block.h` basado en enum.

## Archivos

| Archivo | Descripción |
|---|---|
| `Block.h` | Struct `BlockProperties` y clase `Block` (propiedades, UV, helpers) |
| `BlockRegistry.h` | Singleton `BlockRegistry`: registra y consulta bloques por ID o nombre |
| `WoodBlocks.h/.cpp` | ~120 bloques: Oak, Spruce, Birch, Jungle, Acacia, Dark Oak, Mangrove, Cherry, Pale Oak, Crimson, Warped |
| `StoneBlocks.h/.cpp` | ~80 bloques: Stone, Granite, Diorite, Andesite, Deepslate, Tuff, Blackstone, Basalt, Nether, End, Bedrock |
| `MetalBlocks.h/.cpp` | ~70 bloques: menas, bloques de metal, 16 variantes de cobre con oxidación |
| `RegisterAllBlocks.h` | Helper que llama a todos los registradores en orden |

## Integración en Game.cpp

```cpp
#include "block/RegisterAllBlocks.h"

void Game::init() {
    // Registrar todos los bloques ANTES de crear el mundo
    registerAllBlocks();

    world = std::make_unique<World>(seed);
    // ...
}
```

## Cómo añadir un bloque nuevo

```cpp
// En el .cpp correspondiente (o en uno nuevo):
BlockProperties p;
p.name             = "Mi Bloque";
p.texTop           = {col, row};   // posición en el atlas de 16×16 tiles
p.texSide          = {col, row};
p.texBottom        = {col, row};
p.hardness         = 1.5f;
p.blastResistance  = 6.0f;
p.solid            = true;
p.transparent      = false;

BlockID myId = BlockRegistry::get().registerBlock(p);
```

## Consultar un bloque en tiempo de ejecución

```cpp
// Por ID
const Block* b = getBlock(someId);
if (b) { float h = b->getHardness(); }

// Por nombre
BlockID stoneId = getBlockId("Stone");

// Helpers
if (isSolid(id))       { /* colisión */ }
if (isTransparent(id)) { /* renderizar cara vecina */ }
```

## Estadísticas

| Categoría | Bloques aproximados |
|---|---|
| Madera | ~120 |
| Piedra | ~80 |
| Minerales | ~20 |
| Almacenamiento/Cuarzo | ~18 |
| Cobre (variantes) | ~30 |
| **Total** | **~270** |
