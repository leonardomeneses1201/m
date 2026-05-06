# 👹 entities/

Carpeta con la definición de todas las criaturas (monstruos) del juego.

## Archivos

| Archivo | Descripción |
|---|---|
| `Entity.h` | Clase base: `EntityType`, `EntityBehavior`, `EntityDimension`, `EntityDef` |
| `OverworldMobs.h` | Zombi, Esqueleto, Araña, Creeper, Enderman, Witch, Phantom, Slime... |
| `NetherMobs.h` | Piglin, Blaze, Ghast, Wither Skeleton, Magma Cube, Hoglin... |
| `EndMobs.h` | Shulker, Ender Dragon, Warden (Deep Dark), Bogged, Breeze (1.21) |
| `Entities.h` | **Cabecera maestra** — incluye todo y expone `findEntityDef()` |

## Cómo usar

```cpp
#include "entities/Entities.h"

// Buscar datos de cualquier criatura
const EntityDef* def = findEntityDef(EntityType::CREEPER);
if (def) {
    printf("%s  HP=%.0f  Botín=%s\n", def->name, def->maxHealth, def->loot);
}

// Comprobar si es un jefe
if (isBoss(EntityType::ENDER_DRAGON)) { /* música de jefe */ }

// ¿Spawnea de noche?
if (spawnsAtNight(EntityType::ZOMBIE)) { /* sistema de spawn nocturno */ }
```

## Cómo añadir una criatura nueva

1. Agrega su `EntityType` en `Entity.h`:
```cpp
enum class EntityType {
    ...
    MI_MONSTRUO,
};
```

2. Agrega su `EntityDef` en la tabla correspondiente (ej. `OverworldMobs.h`):
```cpp
{
    "Mi Monstruo",
    EntityType::MI_MONSTRUO,
    EntityBehavior::HOSTILE,
    EntityDimension::OVERWORLD,
    /*HP*/ 20.f, /*daño*/ 4.f, /*velocidad*/ 0.25f,
    "Botín que suelta"
},
```

## Estadísticas actuales

| Dimensión | Criaturas |
|---|---|
| Overworld | 13 |
| Nether | 8 |
| End | 2 |
| Deep Dark | 1 |
| Nuevas 1.21 | 2 |
| **Total** | **26** |
