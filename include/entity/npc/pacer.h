#ifndef PACER_H
#define PACER_H

#include "utils/mathematics.h"

typedef struct {
    Int2D walk;
    bool stuck;
} PacerSoul;

PacerSoul* constructPacerSoul();

#endif
