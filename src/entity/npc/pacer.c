#include <stdlib.h>
#include "../../../include/entity/npc/pacer.h"
#include "../../../include/utils/mathematics.h"
//#include "entity/npc/pacer.h"
//#include "utils/mathematics.h"

PacerSoul* constructPacerSoul() {
    PacerSoul* soul = malloc(sizeof(PacerSoul));
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}