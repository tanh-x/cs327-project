#include <stdlib.h>
#include "../../../include/entity/npc/wanderer.h"
#include "../../../include/world/mapbuilder.h"
#include "../../../include/utils/mathematics.h"
//#include "utils/mathematics.h"
//#include "entity/npc/wanderer.h"
//#include "world/mapbuilder.h"

WandererSoul* constructPacerSoul(TileType birthplace) {
    WandererSoul* soul = malloc(sizeof(WandererSoul));
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    soul->birthplace = birthplace;
    return soul;
}