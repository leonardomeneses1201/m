# 📦 blocks/

Esta carpeta contiene todo lo relacionado con la definición y propiedades de los bloques del juego.

## Archivos

| Archivo | Descripción |
|---|---|
| `Block.h` | Enum `BlockType`, struct `BlockData`, atlas UV, `isSolid()`, `isTransparent()` |
| `BlockTypes.h` | Extensiones: categorías, dureza, resistencia a explosiones, helpers |

## Cómo añadir un bloque nuevo

1. Agrega una entrada al enum `BlockType` en `Block.h` (antes de `COUNT`):
```cpp
enum class BlockType : uint8_t {
    ...
    GRAVEL,   // ← nuevo bloque
    COUNT
};
```

2. Agrega su `BlockData` en el array `BLOCK_DATA[]` (mismo orden que el enum):
```cpp
// GRAVEL  { name, texTop, texSide, texBottom, solid, transparent }
{"Gravel", {10,0},{10,0},{10,0}, true, false},
```

3. El UV `{10, 0}` indica la columna 10, fila 0 en el atlas de 16×16 tiles.  
   Cada tile es 16×16 px dentro de una textura de 256×256 px.

4. Actualiza la hotbar en `Game.cpp` y el `README_Windows.md` si quieres.
