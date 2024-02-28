#ifndef PACER_H
#define PACER_H

#include "utils/mathematics.h"

// Pacers walk in one direction, then turn the opposite way if they hit an uncrossable tile
// * walk: A 2D int vector that stores the current walking direction
typedef struct {
    Int2D walk;
} PacerSoul;

PacerSoul* constructPacerSoul();

#endif
