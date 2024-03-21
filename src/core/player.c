// This file holds functionalities related to the player

#include "core/player.h"
#include "entity/pathfinding.h"
#include "core/game.h"
#include "entity/event.h"

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
    if (cost == UNCROSSABLE || cost < -1) return UNCROSSABLE_TERRAIN;

    // If we got here, it must be a valid move, so immediately move the player and its entity, bypassing the
    // event queue system. We also skip moveEntity's check.
    if (moveEntity(game->entManager, playerEnt, dx, dy)) {
        // After this, the position will have been updated for both the Player and the corresponding entity
        // Next, we must add an PLAYER_INPUT_BLOCKING event to the event queue afterward with the cost.
        Event* event = constructInputBlockingEvent(playerEnt, cost);
        event->resolveTime = game->time + event->cost;
        enqueueEvent(entManager, event);

        // The movement is successful, and nothing else needs to be done, so we return.
        return STANDARD;
    } else {
        // If the return was false, it means we have encountered another entity at this location.
        return ENTITY_ENCOUNTER;
    }
}

void dispatchPlayerEncounter(GameManager* game, PlayerEncounterScenario scenario) {
    Player* player = game->player;
    EntityManager* entManager = game->entManager;

    // Normal movement is already handled within attemptMovePlayer()
    if (scenario == STANDARD) return;

    // Otherwise, enqueue an idle event for the player, and continue with other scenarios
    Event* event = constructInputBlockingEvent(player->currentEntity, PLAYER_SPECIAL_ACTION_IDLE_TIME);
    event->resolveTime = game->time + event->cost;
    enqueueEvent(entManager, event);

    // If we hit uncrossable terrain, just do nothing and wait.
    if (scenario == UNCROSSABLE_TERRAIN) return;
}
