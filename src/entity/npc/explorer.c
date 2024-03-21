#include "core/player.h"
#include "entity/event.h"
#include "entity/pathfinding.h"
#include "entity/npc/explorer.h"
#include "core/game_manager.h"

ExplorerSoul* constructExplorerSoul() {
    ExplorerSoul* soul = malloc(sizeof(ExplorerSoul));
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}

// Explorers walk in one direction, and turns in a random direction if they can't walk forward.
bool explorerMovementAI(Event* event, Entity* entity) {
    Map* map = GAME.world->current;
    ExplorerSoul* soul = entity->soul;
    Int2D* walk = &soul->walk;

    int dx;
    int dy;
    int cost;
    for (int _ = 0; _ < MAX_ITERATIONS_SMALL; _++) {
        // Roll a random direction, standing still for one turn is an option. You gotta take breaks sometimes.
        dx = clamp(randomInt(-2, 2), -1, 1);
        dy = randomInt(-1, 1);
        cost = getTerrainCost(map->tileset[entity->mapY + dy][entity->mapX + dx].type, EXPLORER);

        // If we found a good direction, then start walking that way
        if (cost != UNCROSSABLE) {
            walk->x = dx;
            walk->y = dy;
            break;
        } // Else retry another roll
    }

    // If we unluckily reached 32 rolls without a successful direction, stand still for now.
    if (cost == UNCROSSABLE) return false;

    // Otherwise, we're good to go
    event->cost = cost;
    event->dx = walk->x;
    event->dy = walk->y;
    return true;
}