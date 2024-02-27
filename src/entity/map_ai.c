#include "entity/map_ai.h"
#include "entity/pathfinding.h"
#include "entity/npc/pacer.h"
#include "entity/npc/wanderer.h"

#define PLAYER_MOVEMENT_PROBABILITY 0.042f
#define DEFAULT_IDLE_COST 6

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

            if (value == UNCROSSABLE || value == UNVISITED) continue;

            // If higher, then we're going further away.
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

bool wandererMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
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
            dy = clamp(randomInt(-2, 2), -1, 1);

            // If we found a good direction, then start walking that way
            if (map->tileset[entity->mapY + walk->y][entity->mapX + walk->x].type == soul->birthplace) {
                walk->x = dx;
                walk->y = dy;
                break;
            } // Else loop again
        }
    }  // Else, keep walking

    event->cost = getTerrainCost(soul->birthplace, WANDERER);
    event->dx = walk->x;
    event->dy = walk->y;

    return true;
}

bool sentryMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
}

bool explorerMovementAI(Event* event, Map* map, Player* player, Entity* entity) {
    return false;
}

bool playerPlaceholderAI(Event* event, Map* map, Player* player, Entity* entity) {
    int dx;
    int dy;
    int cost;

    for (int _ = 0;; _++) {
        if (_ >= MAX_ITERATIONS_SMALL) {
            dx = 0;
            dy = 0;
            break;
        }
        // Roll a random direction.
        dx = randomInt(-1, 1);
        dy = randomInt(-1, 1);

        cost = getTerrainCost(map->tileset[player->mapX + dx][player->mapY + dy].type, PLAYER);
        // If we found a good direction, then start walking that way
        if (cost != UNCROSSABLE) break;
    }

    event->cost = cost;
    event->dx = dx;
    event->dy = dy;

    return true;
}


bool (* dispatchMovementAIHandler(EntityType type))(
    Event* event,
    Map* map,
    Player* player,
    Entity* entity
) {
    switch (type) {
        case PLAYER: return playerPlaceholderAI;
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
    event->dx = 0;
    event->dy = 0;
    event->actor = entity;

    bool (* handler)(Event*, Map*, Player*, Entity*) = dispatchMovementAIHandler(entity->type);
    bool success = handler(event, map, player, entity);
    if (!success) return constructIdleEvent(entity, 3);

    return event;
}
