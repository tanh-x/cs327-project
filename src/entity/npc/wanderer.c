#include "core/game_manager.h"
#include "core/player.h"
#include "entity/event.h"
#include "entity/pathfinding.h"
#include "entity/npc/wanderer.h"

WandererSoul* constructWandererSoul(TileType birthplace) {
    WandererSoul* soul = malloc(sizeof(WandererSoul));
    soul->birthplace = birthplace;
    soul->walk.x = randomInt(-1, 1);
    soul->walk.y = randomInt(-1, 1);
    if (soul->walk.x == 0 && soul->walk.y == 0) soul->walk.x = 1;
    return soul;
}

// Wanderers move until they get to the edge of their "birthplace" terrain (stored in the soul), then they turn
// in a random direction.
bool wandererMovementAI(Event* event, Entity* entity) {
    Map* map = GAME.world->current;
    WandererSoul* soul = entity->soul;
    Int2D* walk = &soul->walk;

    TileType nextTileType = map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type;

    int dx;
    int dy;
    if (nextTileType != soul->birthplace || (walk->x == 0 && walk->y == 0)) {
        for (int _ = 0;; _++) {
            if (_ >= MAX_ITERATIONS_SMALL) {
                // If we unluckily reached 32 rolls without a successful direction, stand still for now.
                walk->x = 0;
                walk->y = 0;
                break;
            }
            // Roll a random direction, standing still for one turn is an option. You gotta take breaks sometimes.
            dx = clamp(randomInt(-2, 2), -1, 1);
            dy = randomInt(-1, 1);

            // If we found a good direction, then start walking that way
            if (map->tileset[entity->mapY + dy][entity->mapX + dx].type == soul->birthplace) {
                walk->x = dx;
                walk->y = dy;
                break;
            } // Else retry another roll
        }
    }  // Else, keep walking

    // Otherwise, we're good to go
    event->cost = getTerrainCost(soul->birthplace, WANDERER);
    event->dx = walk->x;
    event->dy = walk->y;
    return true;
}