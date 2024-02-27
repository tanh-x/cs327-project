#include <stdlib.h>
#include "entity/npc/wanderer.h"

WandererSoul* constructWandererSoul(TileType birthplace) {
    WandererSoul* soul = malloc(sizeof(WandererSoul));
    soul->birthplace = birthplace;
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}