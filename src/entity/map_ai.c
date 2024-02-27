#include "entity/map_ai.h"
#include "entity/pathfinding.h"
#include "entity/npc/pacer.h"

bool gradientDescentAI(Event* event, Map* map, Player* player, Entity* entity) {
    DistanceField* field = getOrComputeDistanceField(
        map->memoizedDistanceFields,
        entity->type,
        map,
        player
    );

    // Iterate over 8 adjacent tiles to find where to move
    int gradient = UNCROSSABLE;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            // Entity is at least 1 away from border, so this is safe
            int value = field->map[entity->mapY + dy][entity->mapX + dx];
            if (value >= gradient) continue;

            // Otherwise, this is a candidate direction
            gradient = value;
            event->dx = dx;
            event->dy = dy;
        }
    }

    // Get the cost of this action
    event->cost = getTerrainCost(
        map->tileset[entity->mapY + event->dy][entity->mapX + event->dx].type,
        entity->type
    );

    // Check if none of the 8 adjacent directions are traversable to the entity
    if (event->cost == UNCROSSABLE) return false;

    return true;
}

bool hikerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return gradientDescentAI(event, map, player, entity);
}

bool rivalMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return gradientDescentAI(event, map, player, entity);
}

bool pacerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    PacerSoul* soul = entity->soul;
    Int2D* walk = &soul->walk;

    // Check if the Pacer is stuck. If so, then don't do anything
    if (soul->stuck) return false;

    TileType nextTileType = map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type;

    int cost = getTerrainCost(nextTileType, PACER);
    if (cost == UNCROSSABLE) {
        // Reverse the walking direction
        walk->y *= -1;
        walk->x *= -1;
        TileType tileBehind = map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type;
        cost = getTerrainCost(tileBehind, PACER);
        if (cost == UNCROSSABLE) {
            // We are stuck, so prevent any future pacer action.
            soul->stuck = true;
            walk->x = 0;
            walk->y = 0;
        } // Else, don't do anything, and let it escape the if
    }

    event->cost = cost;
    event->dx = walk->x;
    event->dy = walk->y;

    return true;
}

bool wandererMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
}

bool sentryMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
}

bool explorerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
}


bool (* dispatchMovementAIHandler(EntityType type))(
    Event* event,
    Map* map,
    Player* player,
    Entity* entity
) {
    switch (type) {
        case PLAYER: return NULL;
        case HIKER: return hikerMovementAI;
        case RIVAL: return rivalMovementAI;
        case SWIMMER: return NULL;
        case PACER: return pacerMovementAI;
        case WANDERER: return wandererMovementAI;
        case SENTRY: return sentryMovementAI;
        case EXPLORER: return explorerMovementAI;
    }
}

Event* initializeEventOnTurn(Map* map, Player* player, Entity* entity) {
    // Initialize new event object
    Event* event = malloc(sizeof(Event));
    event->type = MOVEMENT;
    event->actor = entity;

    bool (* handler)(Event*, Map*, Player*, Entity*) = dispatchMovementAIHandler(entity->type);
    bool success = handler(event, map, player, entity);
    if (!success) return NULL;

    return event;
}
