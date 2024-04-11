// This file has components that drive the game, or otherwise is a very centerpiece part of the project
#include <unistd.h>

#include "core/game.hpp"
#include "graphics/renderer.hpp"
#include "entities/event.hpp"
#include "core/input.hpp"
#include "entities/pathfinding.hpp"
#include "context/ctx_battle_view.hpp"


#define BATTLE_INITIATION_COOLDOWN 24
#define NPC_BATTLE_INITIATION_PROBABILITY 0.33f

// The main game loop
void gameLoop() {
    // Pointers to essential components
    GAME.quit_game = false;
    World* world = GAME.world;
    Player* player = GAME.player;

    // Draw once
    renderGameUpdate();
    Event* event;
    while (!GAME.quit_game) {
        Map* map = world->current;
        EntityManager* entManager = GAME.currentEntManager;

        // Note: the first event ever (resolveTime = 0) is always the Player's first event
        while ((event = static_cast<Event*>(heap_remove_min(entManager->eventQueue))) != nullptr) {
            // Time travelling is strictly prohibited
            entManager->eventTime = max(entManager->eventTime, event->resolveTime);

            CorporealEntity* actor = event->actor;

            // If it's a player event, break the event loop.
            if (actor->type == EntityEnum::PLAYER && event->type == PLAYER_INPUT_BLOCKING) break;

            // Delegate the event action to the entity manager
            resolveEvent(event);

            // If the entity is near the player, and they haven't been defeated, and there isn't an active cooldown
            if (actor->activeBattle
                && max(abs(actor->mapX - player->mapX), abs(actor->mapY - player->mapY)) <= 1
                && entManager->eventTime >= entManager->nextBattleInitiationTime
                // Only enter the battle_opponent with 33% probability
                && proba() < NPC_BATTLE_INITIATION_PROBABILITY) {
                // Render the game before entering battle_opponent
                renderGameUpdate();
                usleep(STD_SLOW_FRAME_DELAY);

                // Then enter the battle_opponent
                auto* battleCtx = new BattleViewContext(GAME.context, actor);
                battleCtx->start();
                // Blocking call until the battle_opponent is finished

                // Do another render when we're done
                clear();
                renderGameUpdate();

                // Update the next possible initiation time
                entManager->nextBattleInitiationTime = entManager->eventTime + BATTLE_INITIATION_COOLDOWN;
            }

            // Queue next event for this entity
            Event* newEvent = actor->constructEventOnTurn();
            entManager->enqueueEvent(newEvent);

            // Dispose the old event
            disposeEvent(event);
        }

        // If we got here, it must be the player's turn, so we await input and handle it

        // The input blocking event is no longer needed
        disposeEvent(event);

        // Keep calling handlePlayerInput until it returns true
        while (!handlePlayerInput());

        // Render the game after handling the player event
        renderGameUpdate();

        // Also, the distance field cache must be invalidated in case the player moved or took some other action
        invalidateMemoization((DistanceField**) map->memoizedDistanceFields);
    }
}

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(MapEntryProps entryProps) {
    clear();  // Clear the ncurses render

    Player* player = GAME.player;
    Map* map = GAME.world->current;

    // Set the player position to the specified position in the entryProps
    player->mapX = entryProps.playerSpawnX;
    player->mapY = entryProps.playerSpawnY;

    // Clear all distance fields on this map
    invalidateMemoization((DistanceField**) map->memoizedDistanceFields);

    // Generate a new entity manager if needed
    if (map->entityManager == nullptr) {
        map->entityManager = new EntityManager();
        int numTrainers = static_cast<int>(
            getBaseNumTrainers(map->wildernessLevel)
            + randomFloat(-1.5f, 0.825f)
        );
        if (getEccentricity(map->globalX, map->globalY) > SALIENCY_THRESHOLD) numTrainers += SALIENT_MAP_TRAINER_BONUS;
        map->entityManager->spawnTrainers(map, numTrainers);
    }

    // Load in the new map's entity manager
    EntityManager* entManager = map->entityManager;
    GAME.currentEntManager = entManager;

    // (Re-)initialize the event system
    entManager->reinitializeEventQueue();
    entManager->fillFirstTurns();

    // Set the player vessel entity position to the player's new position (set in move)
    player->possessEntity(entManager->vesselEntity, entManager);
}

// Moves the player to an adjacent map, and makes a call to setupGameOnMapLoad
// Returns the pointer to that map
void moveInWorldDirection(CardinalDir dir) {
    int worldSeed = GAME.world->worldSeed;
    int x = GAME.player->globalX;
    int y = GAME.player->globalY;
    int dx = 0;
    int dy = 0;
    int playerSpawnX;
    int playerSpawnY;

    // Calculates where to spawn the player, by considering the gate position.
    // The gate positions are given by the global hash function
    switch (dir) {
        case NORTH: dy = -1;
            playerSpawnX = hashWithMapCardinalDir(x, y, NORTH, worldSeed);
            playerSpawnY = MAP_HEIGHT - 2;
            break;

        case SOUTH: dy = 1;
            playerSpawnX = hashWithMapCardinalDir(x, y, SOUTH, worldSeed);
            playerSpawnY = 1;
            break;

        case WEST: dx = -1;
            playerSpawnX = MAP_WIDTH - 2;
            playerSpawnY = hashWithMapCardinalDir(x, y, WEST, worldSeed);
            break;

        case EAST: dx = 1;
            playerSpawnX = 1;
            playerSpawnY = hashWithMapCardinalDir(x, y, EAST, worldSeed);
            break;
    }

    // Declare the position of the player to line up with the gate
    MapEntryProps entryProps;
    entryProps.playerSpawnX = playerSpawnX;
    entryProps.playerSpawnY = playerSpawnY;

    // Move there, and check if it succeeded or not
    if (moveToMap(x + dx, y + dy, nullptr) != nullptr) {
        // Delegate the rest of the work to setupGameOnMapLoad
        setupGameOnMapLoad(entryProps);
    }
}

// Moves the player to a map at the specified parameters (globalX, globalY)
// Returns the pointer to that map
Map* moveToMap(int globalX, int globalY, MapEntryProps* entryProps) {
    // Get the new map at the specified coordinates
    Map* newMap = getMap(GAME.world, globalX, globalY, true);

    // Check if it's a valid map, if so then actually move there, otherwise a nullptr will be returned.
    if (newMap != nullptr) {
        GAME.player->globalX = globalX;
        GAME.player->globalY = globalY;
        GAME.world->current = newMap;

        if (entryProps != nullptr) {
            // Place the player entity on the western gate (arbitrary choice)
            entryProps->playerSpawnX = 2;
            entryProps->playerSpawnY = hashWithMapCardinalDir(
                globalX, globalY,
                WEST,
                GAME.world->worldSeed
            );
        }
    }

    return newMap;
}
