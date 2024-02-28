#ifndef WANDERER_H
#define WANDERER_H

#include "utils/mathematics.h"


// Wanderers walk in one direction, and never leaves their spawn terrain.
typedef struct {
    // A 2D int vector that stores the current walking direction
    Int2D walk;

    // The type of tile that the wanderer was spawned on
    TileType birthplace;
} WandererSoul;


WandererSoul* constructWandererSoul(TileType birthplace);

// Wanderers move until they get to the edge of their "birthplace" terrain (stored in the soul), then they turn
// in a random direction.
bool wandererMovementAI(Event* event, Map* map, Player* player, Entity* entity);

#endif
