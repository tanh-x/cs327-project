#include "world/mapbuilder.h"
#include "core/player.h"
#include "entity/event.h"
#include "entity/pathfinding.h"
#include "entity/npc/pacer.h"

PacerSoul* constructPacerSoul() {
    PacerSoul* soul = malloc(sizeof(PacerSoul));
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}

// Pacers move back and forth, turning around whenever they encounter uncrossable terrain
bool pacerMovementAI(Event* event, Map* map, __attribute__((unused)) Player* player, Entity* entity) {
    PacerSoul* soul = entity->soul;
    Int2D* walk = &soul->walk;

    TileType nextTileType = map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type;

    int cost = getTerrainCost(nextTileType, PACER);
    if (cost == UNCROSSABLE) {
        // Reverse the walking direction
        walk->y *= -1;
        walk->x *= -1;
        TileType tileBehind = map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type;
        cost = getTerrainCost(tileBehind, PACER);
        if (cost == UNCROSSABLE) {
            // We are stuck, so try again next turn
            return false;
        } // Else, don't do anything, and let it escape the if
    }

    event->cost = cost;
    event->dx = walk->x;
    event->dy = walk->y;

    return true;
}
