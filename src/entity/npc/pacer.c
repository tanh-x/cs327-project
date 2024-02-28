#include <stdlib.h>
#include "entity/npc/pacer.h"

PacerSoul* constructPacerSoul() {
    PacerSoul* soul = malloc(sizeof(PacerSoul));
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}