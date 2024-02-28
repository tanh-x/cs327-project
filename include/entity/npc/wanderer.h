#ifndef WANDERER_H
#define WANDERER_H

#include "utils/mathematics.h"
#include "world/mapbuilder.h"

// Wanderers walk in one direction, and never leaves their spawn terrain.
// * walk: A 2D int vector that stores the current walking direction
// * birthplace: The type of tile that the wanderer was spawned on
typedef struct {
    Int2D walk;
    TileType birthplace;
} WandererSoul;

WandererSoul* constructWandererSoul(TileType birthplace);

#endif
