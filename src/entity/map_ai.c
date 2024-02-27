#include "entity/map_ai.h"
#include "entity/pathfinding.h"

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
    // Check if none of the 8 adjacent directions are traversable to the entity
    if (gradient == UNCROSSABLE) return false;

    // Get the cost of this action
    event->cost = getTerrainCost(
        map->tileset[entity->mapY + event->dy][entity->mapX + event->dx].type,
        entity->type
    );

    return true;
}

bool hikerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return gradientDescentAI(event, map, player, entity);
}

bool rivalMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return gradientDescentAI(event, map, player, entity);
}

bool pacerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
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
