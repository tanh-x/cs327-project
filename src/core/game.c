// This file has components that drive the game, or otherwise is a very centerpiece part of the project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "core/game.h"
#include "graphics/artist.h"
#include "entity/pathfinding.h"
#include "entity/map_ai.h"
#include "graphics/renderer.h"
#include "core/input.h"

#define RENDER_TIMER_INTERVAL 5

// The main game loop
void gameLoop() {
    // Pointers to essential components
    World* world = GAME.world;
    Map* map = world->current;
    EntityManager* entManager = GAME.entManager;

    // Used to occasionally rerender the game, even if it's not the player's turn yet.
    int renderTimeTarget = RENDER_TIMER_INTERVAL;

    // Draw once
    renderGameUpdate();
    Event* event;
    while (!GAME.quit_game) {
        // Note: the first event ever (resolveTime = 0) is always the Player's first event
        while ((event = heap_remove_min(entManager->eventQueue))) {
            // Time travelling is strictly prohibited
            GAME.time = max(GAME.time, event->resolveTime);

            // If it's a player event, break the event loop.
            if (event->actor->type == PLAYER && event->type == PLAYER_INPUT_BLOCKING) break;

            // Delegate the event action to the entity manager
            resolveEvent(event);

            // Queue next event for this entity
            Event* newEvent = constructEventOnTurn(event->actor);
            newEvent->resolveTime = GAME.time + newEvent->cost;
            enqueueEvent(newEvent);

            // Dispose the old event
            disposeEvent(event);

            // Render it out every once in a while, in case the player did some "time-consuming" action
            if (GAME.time >= renderTimeTarget) {
                renderGameUpdate();
                renderTimeTarget = (GAME.time / RENDER_TIMER_INTERVAL + 1) * RENDER_TIMER_INTERVAL;
                usleep(OPTIONS.frameTimeMicros);
            }
        }

        // If we got here, it must be the player's turn, so we await input and handle it

        // The input blocking event is no longer needed
        disposeEvent(event);

        // Keep calling handlePlayerInput until it returns true
        while (!handlePlayerInput());

        // Render the game after handling the player event
        renderGameUpdate();

        // Also, the distance field cache must be invalidated in case the player moved or took some other action
        invalidateMemoization(map->memoizedDistanceFields);
    }
}

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(MapEntryProps* entryProps) {
    // Clean up previous EntityManager, if any
    if (GAME.entManager != NULL) disposeEntityManager(GAME.entManager);

    // Load useful pointers
    Player* player = GAME.player;
    player->mapX = entryProps->playerSpawnX;
    player->mapY = entryProps->playerSpawnY;
    player->currentEntity = NULL;  // Will soon be loaded
    Map* map = GAME.world->current;
    invalidateMemoization(map->memoizedDistanceFields);

    // Load in new entity manager. It will also assign a new entity to the player
    initializeEntityManager(OPTIONS.numTrainers);
    GAME.time = 0;

    // Place trainers on the map
    // Possible trainer types
    EntityType types[] = {HIKER, RIVAL, PACER, WANDERER, SENTRY, EXPLORER};
    int numTypes = 6;

    // Start placing numTrainers NPCs, or until we stop prematurely if it got too crowded
    for (int i = 0; i < OPTIONS.numTrainers; i++) {
        EntityType entType;
        Entity* entity = NULL;

        // Keep retrying to place the trainer until we land on a valid spot
        for (int _ = 0; _ < MAX_ITERATIONS && entity == NULL; _++) {
            // Get the type of the new trainer
            if (i == 0) entType = HIKER;
            else if (i == 1) entType = RIVAL;
            else entType = types[randomInt(0, numTypes - 1)];

            // Don't spawn the NPC on the border or right next to it
            // While we can do (1, -2) instead, it's just my preference to leave a 1 tile padding
            int x = randomInt(2, MAP_WIDTH - 3);
            int y = randomInt(2, MAP_HEIGHT - 3);

            // Check if the terrain cost was infinite
            if (getTerrainCost(map->tileset[y][x].type, entType) == UNCROSSABLE) continue;

            // spawnEntity might return NULL, indicating an unsuccessful placement, so entity = NULL and we try again
            entity = spawnEntity(entType, x, y);
        }

        // If we already went through MAX_ITERATIONS and entity is still NULL, give up
        if (entity == NULL) break;

        // Else, we got a successful trainer placement.

        // Make them follow and try to fight the player
        entity->activeBattle = true;

        // Try creating and queueing a new event.
        Event* event = constructEventOnTurn(entity);
        if (event == NULL) continue;
        // Event initialization was successful, so we add it to the queue
        event->resolveTime = GAME.time + event->cost;
        enqueueEvent(event);
    }
}

// Moves the player to an adjacent map
// Returns the pointer to that map
Map* moveInWorldDirection(char dir, MapEntryProps* entryProps) {
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
        case 'n':dy = -1;
            playerSpawnX = hashWithMapCardinalDir(x, y, NORTH, worldSeed);
            playerSpawnY = MAP_HEIGHT - 2;
            break;

        case 's':dy = 1;
            playerSpawnX = hashWithMapCardinalDir(x, y, SOUTH, worldSeed);
            playerSpawnY = 1;
            break;

        case 'w':dx = -1;
            playerSpawnX = MAP_WIDTH - 2;
            playerSpawnY = hashWithMapCardinalDir(x, y, WEST, worldSeed);
            break;

        case 'e':dx = 1;
            playerSpawnX = 1;
            playerSpawnY = hashWithMapCardinalDir(x, y, EAST, worldSeed);
            break;

        default:printf("Something is very wrong [core/game.c->moveInWorldDirection()]");
            exit(1);
    }

    // Move there
    Map* newMap = moveToMap(x + dx, y + dy, entryProps);
    entryProps->playerSpawnX = playerSpawnX;
    entryProps->playerSpawnY = playerSpawnY;
    return newMap;
}

// Moves the player to a map at the specified parameters (globalX, globalY)
// Returns the pointer to that map
Map* moveToMap(int globalX, int globalY, MapEntryProps* entryProps) {
    // Get the new map at the specified coordinates
    Map* newMap = getMap(GAME.world, entryProps, globalX, globalY, true);

    // Check if it's a valid map, if so then actually move there, otherwise a NULL will be returned.
    if (newMap != NULL) {
        GAME.player->globalX = globalX;
        GAME.player->globalY = globalY;
        GAME.world->current = newMap;
    }
    return newMap;
}
