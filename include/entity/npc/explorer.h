#ifndef EXPLORER_H
#define EXPLORER_H

#include "utils/mathematics.h"


// Explorers walk in one direction, and turns in a random direction if they can't walk forward.
typedef struct {
    // A 2D int vector that stores the current walking direction
    Int2D walk;
} ExplorerSoul;


ExplorerSoul* constructExplorerSoul();

bool explorerMovementAI(Event* event, Map* map, Player* player, Entity* entity);

#endif
