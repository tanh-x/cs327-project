// This file holds functionalities related to the player

#include "core/game.hpp"
#include "entity/entity_manager.hpp"
#include "contexts/ctx_battle_view.hpp"
#include "entity/pathfinding.hpp"

void dispatchPlayerEncounter(EncounterScenario scenario) {
    // This shouldn't happen, as the call location is inside an if-statement guarding against this.
    if (scenario == EncounterScenario::INVALID) return;

    Player* player = GAME.player;
    EntityManager* entManager = GAME.entManager;

    // Normal movement is already handled within attemptMove()
    if (scenario == EncounterScenario::STANDARD) return;

    // Otherwise, enqueue an idle event for the player, and continue with other scenarios
    enqueueInputBlockingEvent(PLAYER_SPECIAL_ACTION_IDLE_TIME);

    // If we hit uncrossable terrain, just do nothing and wait.
    if (scenario == EncounterScenario::UNCROSSABLE_TERRAIN) return;

    // If we get an entity encounter, try start a battle
    if (scenario == EncounterScenario::ENTITY_ENCOUNTER) {
        Entity* opponent = entManager->entMap[player->mapY][player->mapX];

        if (opponent != nullptr && opponent->activeBattle) {
            enterPlaceholderBattle(opponent);
        }

        player->mapX = player->currentEntity->mapX;
        player->mapY = player->currentEntity->mapY;
    }
}

// Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE
// or out of bounds.
EncounterScenario Player::attemptMove(int dx, int dy) {
    Entity* playerEnt = this->currentEntity;
    int newX = this->mapX + dx;
    int newY = this->mapY + dy;

    // Check if out of bounds or coincides with the border
    if (!isInsideMapBorders(newX, newY)) return EncounterScenario::UNCROSSABLE_TERRAIN;

    // Check if the terrain we're moving towards is traversable to the player
    int cost = getTerrainCost(GAME.world->current->tileset[newY][newX].type, EntityEnum::PLAYER);
    if (cost == UNCROSSABLE || cost < -1) return EncounterScenario::UNCROSSABLE_TERRAIN;

    // If we got here, it must be a valid move, so immediately move the player and its entity, bypassing the
    // event queue system. We also skip move's check.
    if (playerEnt->moveBy(dx, dy)) {
        // After this, the position will have been updated for both the Player and the corresponding entity
        // Next, we must add an PLAYER_INPUT_BLOCKING event to the event queue afterward with the cost.
        enqueueInputBlockingEvent(cost);

        // The movement is successful, and nothing else needs to be done, so we return.
        return EncounterScenario::STANDARD;
    } else {
        // If the return was false, it means we have encountered another entity at this location.
        // Set the player to be at this location, but not the player entity.
        // We will rectify this difference once the battle is complete
        this->mapX = newX;
        this->mapY = newY;
        return EncounterScenario::ENTITY_ENCOUNTER;
    }
}
