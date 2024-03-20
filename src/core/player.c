// This file holds functionalities related to the player

#include "core/player.h"
#include "entity/pathfinding.h"
#include "core/game.h"

#define PLAYER_IDLE_TIME 10
// Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE or out of bounds.

PlayerEncounterScenario attemptMovePlayer(GameManager* game, int dx, int dy) {
    Player* player = game->player;
    EntityManager* entManager = game->entManager;
    Entity* playerEnt = player->currentEntity;
    int newX = player->mapX + dx;
    int newY = player->mapY + dy;

    // Check if out of bounds or coincides with the border
    if (!isInsideMapBorders(newX, newY)) return UNCROSSABLE_TERRAIN;

    // Check if the terrain we're moving towards is traversable to the player
    int cost = getTerrainCost(game->world->current->tileset[newY][newX].type, PLAYER);
    if (cost == UNCROSSABLE || cost < -1) {
        // If so, then make the player idle for 10 units of time
        Event* event = constructInputBlockingEvent(playerEnt, PLAYER_IDLE_TIME);
        event->resolveTime = game->time + event->cost;
        enqueueEvent(entManager, event);

        // No movement, so no other actions for now.
        return UNCROSSABLE_TERRAIN;
    }

    // If we got here, it must be a valid move, so immediately move the player and its entity, bypassing the
    // event queue system. We also skip moveEntity's check.
    if (moveEntity(game->entManager, playerEnt, dx, dy)) {
        // After this, the position will have been updated for both the Player and the corresponding entity
        // Next, we must add an PLAYER_INPUT_BLOCKING event to the event queue afterward with the cost.
        Event* event = constructInputBlockingEvent(playerEnt, cost);
        event->resolveTime = game->time + event->cost;
        enqueueEvent(entManager, event);

        // The movement is successful, and nothing else needs to be done, so we return.
        return NONE;
    } else {
        // If the return was false, it means we have encountered another entity at this location.
        return ENTITY_ENCOUNTER;
    }
}