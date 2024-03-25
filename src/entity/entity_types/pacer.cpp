#include "entity/entities.hpp"
#include "entity/npc/pacer.hpp"
#include "core/game.hpp"
#include "entity/pathfinding.hpp"

Pacer::Pacer(int x, int y) : Entity(EntityEnum::PACER, x, y) {
    this->walk.x = randomInt(-1, 1);
    this->walk.y = randomInt(-1, 1);
    if (this->walk.x == 0 && this->walk.y == 0) this->walk.x = 1;
}

// Pacers move back and forth, turning around whenever they encounter uncrossable terrain
bool Pacer::moveAI(Event* event) {
    Map* map = GAME.world->current;

    TileType nextTileType = map->tileset[mapY + walk.y][mapX + walk.x].type;

    int cost = getTerrainCost(nextTileType, EntityEnum::PACER);
    if (cost == UNCROSSABLE) {
        // Reverse the walking direction
        walk.y *= -1;
        walk.x *= -1;
        TileType tileBehind = map->tileset[mapY + walk.y][mapX + walk.x].type;
        cost = getTerrainCost(tileBehind, EntityEnum::PACER);
        if (cost == UNCROSSABLE) {
            // We are stuck, so try again next turn
            return false;
        } // Else, don't do anything, and let it escape the if
    }

    event->cost = cost;
    event->dx = walk.x;
    event->dy = walk.y;

    return true;
}
