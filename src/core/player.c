// This file holds functionalities related to the player

#include "core/player.h"
#include "entity/pathfinding.h"
#include "core/game.h"
#include "entity/event.h"
#include "contexts/ctx_battle_view.h"

// Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE or out of bounds.
PlayerEncounterScenario attemptMovePlayer(int dx, int dy) {
    Player* player = GAME.player;
    Entity* playerEnt = player->currentEntity;
    int newX = player->mapX + dx;
    int newY = player->mapY + dy;

    // Check if out of bounds or coincides with the border
    if (!isInsideMapBorders(newX, newY)) return UNCROSSABLE_TERRAIN;

    // Check if the terrain we're moving towards is traversable to the player
    int cost = getTerrainCost(GAME.world->current->tileset[newY][newX].type, PLAYER);
    if (cost == UNCROSSABLE || cost < -1) return UNCROSSABLE_TERRAIN;

    // If we got here, it must be a valid move, so immediately move the player and its entity, bypassing the
    // event queue system. We also skip moveEntity's check.
    if (moveEntity(playerEnt, dx, dy)) {
        // After this, the position will have been updated for both the Player and the corresponding entity
        // Next, we must add an PLAYER_INPUT_BLOCKING event to the event queue afterward with the cost.
        enqueueInputBlockingEvent(cost);

        // The movement is successful, and nothing else needs to be done, so we return.
        return STANDARD;
    } else {
        // If the return was false, it means we have encountered another entity at this location.
        // Set the player to be at this location, but not the player entity
        player->mapX = newX;
        player->mapY = newY;
        return ENTITY_ENCOUNTER;
    }
}

void dispatchPlayerEncounter(PlayerEncounterScenario scenario) {
    // This shouldn't happen, as the call location is inside a if-statement guarding against this.
    if (scenario == INVALID) return;

    Player* player = GAME.player;
    EntityManager* entManager = GAME.entManager;

    // Normal movement is already handled within attemptMovePlayer()
    if (scenario == STANDARD) return;

    // Otherwise, enqueue an idle event for the player, and continue with other scenarios
    enqueueInputBlockingEvent(PLAYER_SPECIAL_ACTION_IDLE_TIME);

    // If we hit uncrossable terrain, just do nothing and wait.
    if (scenario == UNCROSSABLE_TERRAIN) return;

    // If we get an entity encounter, try start a battle
    if (scenario == ENTITY_ENCOUNTER) {
        Entity* opponent = entManager->entMap[player->mapY][player->mapX];

        if (opponent != NULL && opponent->activeBattle) {
            enterPlaceholderBattle(opponent);
        }

        player->mapX = player->currentEntity->mapX;
        player->mapY = player->currentEntity->mapY;
    }
}
