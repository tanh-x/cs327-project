
#include "utils/mathematics.hpp"
#include "entities/ai/map_ai.hpp"
#include "entities/pathfinding.hpp"
#include "core/game_manager.hpp"

#define TIE_BREAKING_PROBABILITY 0.3f

// Gradient descent based movement AI
// Gets or computes a distance field corresponding to the entity type, which is generated via Dijkstra's algorithm.
// Then, find a direction that minimizes the distance to the source (the player), essentially making the entity follows
// the shortest accessible path to the player one on every turn.
// This algorithm is used by hikers and rivals.
bool gradientDescentAI(Event* event, CorporealEntity* entity) {
    Map* map = GAME.world->current;
    Player* player = GAME.player;

    DistanceField* field = getOrComputeDistanceField(
        map->memoizedDistanceFields,
        entity->type,
        map,
        player
    );

    // Iterate over 8 adjacent tiles to find where to MOVE
    int gradient = UNCROSSABLE;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            // AbstractEntity is at least 1 away from out of bounds, so this is safe
            int value = field->map[entity->mapY + dy][entity->mapX + dx];

            if (value == UNCROSSABLE || value == UNVISITED) continue;

            // If higher, then we're going further away.
            if (value > gradient) continue;

            // If there is a tie, then take the new direction with 70% probability
            if (value == gradient && proba() < TIE_BREAKING_PROBABILITY) continue;

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

// Creates a new event according to the entity's AI
//Event* constructEventOnTurn(AbstractEntity* entity) {
// // Moved to AbstractEntity::constructEventOnTurn()
//}
//

// Takes in an EntityEnum, and returns a pointer to a function, called a movement AI handler, corresponding to the
// given entity type's movement AI.
// A movement AI handler is a function that takes in 4 arguments: event, map, player, entity; and returns a boolean
// that indicates whether the event creation was successful or not.
// The event will be of movement type, with dx, dy, and cost correctly written into.
//bool (* dispatchMovementAIHandler(EntityEnum type))(Event* event, AbstractEntity* entity) {
//    switch (type) {
//        case HIKER: return gradientDescentAI;
//        case RIVAL: return gradientDescentAI;
//        case SWIMMER: return nullptr;
//        case PACER: return pacerMovementAI;
//        case WANDERER: return wandererMovementAI;
//        case SENTRY: return sentryMovementAI;
//        case EXPLORER: return explorerMovementAI;
//        default: return nullptr;
//    }
//}