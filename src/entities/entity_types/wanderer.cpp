#include "core/game_manager.hpp"
#include "core/player.hpp"
#include "entities/event.hpp"
#include "entities/pathfinding.hpp"
#include "entities/entity_types//wanderer.hpp"

Wanderer::Wanderer(int x, int y) : CorporealEntity(EntityEnum::WANDERER, x, y) {
    this->birthplace = GAME.world->current->tileset[y][x].type;
    this->walk.x = randomInt(-1, 1);
    this->walk.y = randomInt(-1, 1);
    if (this->walk.x == 0 && this->walk.y == 0) this->walk.x = 1;
}

// Wanderers move until they get to the edge of their "birthplace" terrain (stored in the soul), then they turn
// in a random direction.
bool Wanderer::moveAI(Event* event) {
    Map* map = GAME.world->current;

    TileType nextTileType = map->tileset[mapY + walk.y][mapX + walk.x].type;

    int dx;
    int dy;
    if (nextTileType != birthplace || (walk.x == 0 && walk.y == 0)) {
        for (int _ = 0;; _++) {
            if (_ >= MAX_ITERATIONS_SMALL) {
                // If we unluckily reached 32 rolls without a successful direction, stand still for now.
                walk.x = 0;
                walk.y = 0;
                break;
            }
            // Roll a random direction, standing still for one turn is an option. You gotta take breaks sometimes.
            dx = clamp(randomInt(-2, 2), -1, 1);
            dy = randomInt(-1, 1);

            // If we found a good direction, then start walking that way
            if (map->tileset[mapY + dy][mapX + dx].type == birthplace) {
                walk.x = dx;
                walk.y = dy;
                break;
            } // Else retry another roll
        }
    }  // Else, keep walking

    // Otherwise, we're good to go
    event->cost = getTerrainCost(birthplace, EntityEnum::WANDERER);
    event->dx = walk.x;
    event->dy = walk.y;
    return true;
}
