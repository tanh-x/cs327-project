#ifndef WANDERER_H
#define WANDERER_H

#include "utils/mathematics.h"
#include "world/mapbuilder.h"

typedef struct {
    Int2D walk;
    TileType birthplace;
} WandererSoul;

WandererSoul* constructWandererSoul(TileType birthplace);

#endif
