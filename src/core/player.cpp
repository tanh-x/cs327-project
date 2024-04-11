// This file holds functionalities related to the player

#include "core/player.hpp"
#include "core/game.hpp"
#include "entities/event.hpp"
#include "context/ctx_battle_view.hpp"
#include "entities/pathfinding.hpp"

void dispatchPlayerEncounter(EncounterScenario scenario) {
    // This shouldn't happen, as the call location is inside an if-statement guarding against this.
    if (scenario == EncounterScenario::INVALID) return;

    Player* player = GAME.player;
    EntityManager* entManager = GAME.currentEntManager;

    // Normal movement is already handled within attemptMove()
    if (scenario == EncounterScenario::STANDARD) return;


    // Check if the player can go to an adjacent map
    if (scenario == EncounterScenario::BORDER_APPROACH) {
        Map* map = GAME.world->current;

        // Check the tile type of the 4 neighboring cells
        // Since gates can't lie on the corner, the player will never be next to two gates at the same time
        if (map->tileset[player->mapY - 1][player->mapX].type == GATE) moveInWorldDirection(NORTH);
        else if (map->tileset[player->mapY + 1][player->mapX].type == GATE) moveInWorldDirection(SOUTH);
        else if (map->tileset[player->mapY][player->mapX - 1].type == GATE) moveInWorldDirection(WEST);
        else if (map->tileset[player->mapY][player->mapX + 1].type == GATE) moveInWorldDirection(EAST);
        else {
            // Otherwise, we just hit the border, so just make it return
            enqueueInputBlockingEvent(PLAYER_SPECIAL_ACTION_IDLE_TIME);
            return;
        }

        // A successful call to any of the world movement functions will land us here

        // Play a funny animation

        // We're already done here, so move on to rerender in game.cpp
        return;
    }

    // Otherwise, enqueue an idle event for the player, and continue with other scenarios
    enqueueInputBlockingEvent(PLAYER_SPECIAL_ACTION_IDLE_TIME);

    // If we hit uncrossable terrain, just do nothing and wait.
    if (scenario == EncounterScenario::UNCROSSABLE_TERRAIN) return;

    // If we get an entity encounter, try start a battle_opponent
    if (scenario == EncounterScenario::ENTITY_ENCOUNTER) {
        CorporealEntity* opponent = entManager->entMap[player->mapY][player->mapX];

        // Check if the opponent can be fought
        if (opponent != nullptr && opponent->activeBattle) {
            auto* battleCtx = new BattleViewContext(GAME.context, opponent);
            battleCtx->start();
            // Blocking call until the battle_opponent is finished
        }

        player->mapX = player->currentEntity->mapX;
        player->mapY = player->currentEntity->mapY;
    }
}

// Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE
// or out of bounds.
EncounterScenario Player::attemptMove(int dx, int dy) {
    PlayerVessel* playerEnt = this->currentEntity;
    int newX = this->mapX + dx;
    int newY = this->mapY + dy;

    // Check if out of bounds or coincides with the border
    if (!isInsideMapBorders(newX, newY)) return EncounterScenario::BORDER_APPROACH;

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
    }

    // If the return was false, it means we have encountered another entity at this location.
    // Set the player to be at this location, but not the player entity.
    // We will rectify this difference once the battle_opponent is complete
    this->mapX = newX;
    this->mapY = newY;
    return EncounterScenario::ENTITY_ENCOUNTER;
}

void Player::possessEntity(PlayerVessel* entity, EntityManager* entManager) {
    entManager->entMap[entity->mapY][entity->mapX] = nullptr;
    entity->mapX = mapX;
    entity->mapY = mapY;
    entManager->entMap[mapY][mapX] = entity;
    currentEntity = entity;
}
