#include "core/player.hpp"
#include "entities/event.hpp"
#include "entities/pathfinding.hpp"
#include "core/game_manager.hpp"
#include "entities/entity_types/explorer.hpp"


Explorer::Explorer(int x, int y) : CorporealEntity(EntityEnum::EXPLORER, x, y) {
    this->walk.x = randomInt(-1, 1);
    this->walk.y = randomInt(-1, 1);
    if (this->walk.x == 0 && this->walk.y == 0) this->walk.x = 1;
}

bool Explorer::moveAI(Event* event) {
    Map* map = GAME.world->current;
    int dx;
    int dy;
    int cost;

    for (int _ = 0; _ < MAX_ITERATIONS_SMALL; _++) {
        // Roll a random direction, standing still for one turn is an option. You gotta take breaks sometimes.
        dx = clamp(randomInt(-2, 2), -1, 1);
        dy = randomInt(-1, 1);
        cost = getTerrainCost(map->tileset[mapY + dy][mapX + dx].type, EntityEnum::EXPLORER);

        // If we found a good direction, then start walking that way
        if (cost != UNCROSSABLE) {
            walk.x = dx;
            walk.y = dy;
            break;
        } // Else retry another roll
    }

    // If we unluckily reached 32 rolls without a successful direction, stand still for now.
    if (cost == UNCROSSABLE) return false;

    // Otherwise, we're good to go
    event->cost = cost;
    event->dx = walk.x;
    event->dy = walk.y;
    return true;
}
