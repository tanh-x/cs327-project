// This file has components that drive the game, or otherwise is a very centerpiece part of the project

#include <unistd.h>
#include "core/game.hpp"
#include "graphics/artist.hpp"
#include "entity/pathfinding.hpp"
#include "entity/map_ai.hpp"
#include "graphics/renderer.hpp"
#include "core/input.hpp"
#include "contexts/ctx_battle_view.hpp"
#include "entity/entity_manager.hpp"

#define RENDER_TIMER_INTERVAL 5
#define BATTLE_INITIATION_COOLDOWN 14

// The main game loop
void gameLoop() {
    // Pointers to essential components
    GAME.quit_game = false;
    World* world = GAME.world;
    Player* player = GAME.player;
    Map* map = world->current;
    EntityManager* entManager = GAME.entManager;

    // Used to occasionally rerender the game, even if it's not the player's turn yet.
    int renderTimeTarget = RENDER_TIMER_INTERVAL;

    // Used to prevent NPCs from spamming too many battle intiations with the player
    int nextBattleInitiationTime = 0;

    // Draw once
    renderGameUpdate();
    Event* event;
    while (!GAME.quit_game) {
        // Note: the first event ever (resolveTime = 0) is always the Player's first event
        while ((event = static_cast<Event*>(heap_remove_min(entManager->eventQueue)))) {
            // Time travelling is strictly prohibited
            GAME.time = max(GAME.time, event->resolveTime);

            Entity* actor = event->actor;

            // If it's a player event, break the event loop.
            if (actor->type == PLAYER && event->type == PLAYER_INPUT_BLOCKING) break;

            // Delegate the event action to the entity manager
            resolveEvent(event);

            // If the entity is near the player, and they haven't been defeated, and there isn't an active cooldown
            if (actor->activeBattle
                && manhattan_dist(actor->mapX, actor->mapY, player->mapX, player->mapY) <= 1
                && GAME.time >= nextBattleInitiationTime) {

                // Render the game before entering battle
                renderGameUpdate();
                usleep(STD_SLOW_FRAME_DELAY);

                // Then enter the battle
                enterPlaceholderBattle(actor);

                // Do another render when we're done
                clear();
                renderGameUpdate();

                // Update the next possible initiation time
                nextBattleInitiationTime = GAME.time + BATTLE_INITIATION_COOLDOWN;
            }

            // Queue next event for this entity
            Event* newEvent = constructEventOnTurn(actor);
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
    // TODO: Put the manager in a map
    // Clean up previous EntityManager, if any
    if (GAME.entManager != nullptr) disposeEntityManager(GAME.entManager);

    // Load useful pointers
    Player* player = GAME.player;
    player->mapX = entryProps->playerSpawnX;
    player->mapY = entryProps->playerSpawnY;
    player->currentEntity = nullptr;  // Will soon be loaded
    Map* map = GAME.world->current;
    invalidateMemoization(map->memoizedDistanceFields);

    // Load in new entity manager. It will also assign a new entity to the player
    GAME.entManager = new EntityManager(OPTIONS.numTrainers);
    GAME.time = 0;

    // Place trainers on the map
    // Possible trainer types
    EntityEnum types[] = {HIKER, RIVAL, PACER, WANDERER, SENTRY, EXPLORER};
    int numTypes = 6;

    // Start placing numTrainers NPCs, or until we stop prematurely if it got too crowded
    for (int i = 0; i < OPTIONS.numTrainers; i++) {
        EntityEnum entType;
        Entity* entity = nullptr;

        // Keep retrying to place the trainer until we land on a valid spot
        for (int _ = 0; _ < MAX_ITERATIONS && entity == nullptr; _++) {
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

            entity = new Entity(entType, x, y);
            if (entity != NULL) GAME.entManager->addEntity(entity);

            // spawnEntity might return NULL, indicating an unsuccessful placement
            // if so then entity = nullptr and we try again
        }

        // If we already went through MAX_ITERATIONS and entity is still NULL, give up
        if (entity == nullptr) break;

        // Else, we got a successful trainer placement.

        // Make them follow and try to fight the player
        entity->activeBattle = true;

        // Try creating and queueing a new event.
        Event* event = constructEventOnTurn(entity);
        if (event == nullptr) continue;
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
    if (newMap != nullptr) {
        GAME.player->globalX = globalX;
        GAME.player->globalY = globalY;
        GAME.world->current = newMap;
    }
    return newMap;
}
