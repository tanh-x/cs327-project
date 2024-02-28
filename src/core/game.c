// This file has components that drive the game, or otherwise is a very centerpiece part of the project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "core/game.h"
#include "graphics/artist.h"
#include "entity/pathfinding.h"
#include "entity/map_ai.h"

#define AWAIT_PLAYER_INPUT true

// The main game loop
void update(GameManager* game, GameOptions* options) {
    // Pointers to essential components
    World* world = game->world;
    Player* player = game->player;
    Map* map = world->current;
    EntityManager* entManager = game->entManager;

    // Draw once
    draw(game, options);

    Event* event;
    while ((event = heap_remove_min(entManager->eventQueue))) {
        // Time travelling is strictly prohibited
        game->time = max(game->time, event->resolveTime);

        // Delegate the event action to the entity manager
        resolveEvent(entManager, event);

        // Redraw if it was a player event
        if (event->actor->type == PLAYER) {
            draw(game, options);
            usleep(options->frameTimeMicros);

            // Special handling after every player event
            invalidateMemoization(map->memoizedDistanceFields);
        }

        // Queue next event for this entity
        Event* newEvent = constructEventOnTurn(map, player, event->actor);
        // newEvent is null-safe
        newEvent->resolveTime = game->time + newEvent->cost;
        enqueueEvent(game->entManager, newEvent);

        // Dispose the old event
        disposeEvent(event);

        if (game->time > 180) return;
    }
}

// (Re)draws the frame
void draw(GameManager* game, GameOptions* options) {
    printf(CLEAR_SCREEN);

    // Print the map
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(game, mapStr);
    prettyPrint(mapStr, options->doColoring);

    // Print the 3 lines
    printf("Map position: (%d, %d)\n", game->player->globalX, game->player->globalY);
    printf("PC position: (%d, %d)\n", game->player->mapX, game->player->mapY);
    printf("Event time: %d\n", game->time);
}

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(GameManager* game, MapEntryProps* entryProps, GameOptions* options) {
    // Clean up previous EntityManager, if any
    if (game->entManager != NULL) disposeEntityManager(game->entManager);

    // Load useful pointers
    Player* player = game->player;
    player->mapX = entryProps->playerSpawnX;
    player->mapY = entryProps->playerSpawnY;
    Map* map = game->world->current;
    invalidateMemoization(map->memoizedDistanceFields);

    // Load in new entity manager
    initializeEntityManager(game);
    game->time = 0;

    // Place trainers on the map
    // Possible trainer types
    EntityType types[] = {HIKER, RIVAL, PACER, WANDERER, SENTRY, EXPLORER};
    int numTypes = 6;

    // Start placing numTrainers NPCs, or until we stop prematurely if it got too crowded
    for (int i = 0; i < options->numTrainers; i++) {
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
            entity = spawnEntity(game, entType, x, y);
        }

        // If we already went through MAX_ITERATIONS and entity is still NULL, give up
        if (entity == NULL) break;

        // Else, we got a successful trainer placement.
        // Try creating and queueing a new event.
        Event* event = constructEventOnTurn(map, player, entity);
        if (event == NULL) continue;
        // Event initialization was successful, so we add it to the queue
        event->resolveTime = game->time + event->cost;
        enqueueEvent(game->entManager, event);
    }
}

// Moves the player to an adjacent map
// Returns the pointer to that map
Map* moveInWorldDirection(GameManager* game, char dir, MapEntryProps* entryProps) {
    int worldSeed = game->world->worldSeed;
    int x = game->player->globalX;
    int y = game->player->globalY;
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
    Map* newMap = moveToMap(game, x + dx, y + dy, entryProps);
    entryProps->playerSpawnX = playerSpawnX;
    entryProps->playerSpawnY = playerSpawnY;
    return newMap;
}

// Moves the player to a map at the specified parameters (globalX, globalY)
// Returns the pointer to that map
Map* moveToMap(GameManager* game, int globalX, int globalY, MapEntryProps* entryProps) {
    // Get the new map at the specified coordinates
    Map* newMap = getMap(game->world, entryProps, globalX, globalY, true);

    // Check if it's a valid map, if so then actually move there, otherwise a NULL will be returned.
    if (newMap != NULL) {
        game->player->globalX = globalX;
        game->player->globalY = globalY;
        game->world->current = newMap;
    }
    return newMap;
}

// Game loop for assignment 1.03 and before
__attribute__((unused)) void update_old(GameManager* game, GameOptions* options) {
    World* world = game->world;
    Player* player = game->player;
    Map* map = world->current;

    bool quitFlag = false;
    while (true) {
        if (quitFlag) break;

        // Start graphics
        char* promptOverride = NULL;

        // Print the map
        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
        worldToString(game, mapStr);
        prettyPrint(mapStr, options->doColoring);

        // First line
        printf("Map position: (%d, %d)", player->globalX, player->globalY);
        printf("\n");

        // Second line
        printf("[ NOTE ] Use `h` for hiker map and `r` for rival map. Or `hh`/`rr` for div 10\n");

        // Third line
        char cmd[CMD_MAX_LENGTH];
        char first;
        printf("Input command: ");
        while (AWAIT_PLAYER_INPUT) {
            fgets(cmd, CMD_MAX_LENGTH, stdin);
            first = cmd[0];

            // Process the input
            if (first == 'q') {
                // q: Quit game
                quitFlag = true;
                break;

            } else if (first == 'n' || first == 'w' || first == 's' || first == 'e') {
                // n,s,w,e: Move along the world in cardinal directions
                MapEntryProps entryProps;
                map = moveInWorldDirection(game, cmd[0], &entryProps);
                setupGameOnMapLoad(game, &entryProps, options);
                if (map != NULL) break;  // NULL only if invalid move

                // Else, it was an invalid move, so let it exit the if body,
                promptOverride = "You reached the map border, try again: ";

            } else if (first == 'f') {
                // f <x> <y>: Fly to a specific map
                int x, y;
                int parsedItems = sscanf(cmd, "f %d %d", &x, &y); // NOLINT(*-err34-c)
                if (parsedItems == 2) {
                    MapEntryProps entryProps;
                    map = moveToMap(game, x, y, &entryProps);
                    entryProps.playerSpawnX = 0;
                    entryProps.playerSpawnY = hashWithMapCardinalDir(x, y, WEST, world->worldSeed);
                    setupGameOnMapLoad(game, &entryProps, options);
                    if (map != NULL) break;  // NULL if invalid move
                    // Else, let it exit the if body
                }

            } else if (first == 'h') {
                // h: Hiker distance map
                DistanceField* distanceField = generateDistanceField(map, player->mapX, player->mapY, HIKER);

                if (cmd[1] == 'h') {
                    printDistanceFieldAlt(distanceField);
                } else {
                    printDistanceField(distanceField);
                }

                promptOverride = "Printed Hiker's distance map. Input command: ";

            } else if (first == 'r') {
                // r: Rival distance map
                DistanceField* distanceField = generateDistanceField(map, player->mapX, player->mapY, RIVAL);

                if (cmd[1] == 'r') {
                    printDistanceFieldAlt(distanceField);
                } else {
                    printDistanceField(distanceField);
                }

                promptOverride = "Printed Rival's distance map. Input command: ";
            }

            // If we got here, it must have been an invalid command
//            printf(CLEAR_PREV_LINE);
            if (promptOverride == NULL) printf("Invalid command, try again: ");
            else {
                printf("%s", promptOverride);
                promptOverride = NULL;
            }
        }
    }
}

