#ifndef PACER_H
#define PACER_H

#include "utils/mathematics.h"


// Pacers walk in one direction, then turn the opposite way if they hit an uncrossable tile
typedef struct {
    // A 2D int vector that stores the current walking direction
    Int2D walk;
} PacerSoul;


PacerSoul* constructPacerSoul();

// Pacers move back and forth, turning around whenever they encounter uncrossable terrain
bool pacerMovementAI(Event* event, Map* map, Player* player, Entity* entity);

#endif
