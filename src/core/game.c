#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/game.h"
#include "graphics/artist.h"
#include "entity/pathfinding.h"

#define MAX_ITERATIONS 10000

void update(GameManager *game, GameOptions *options) {
    World *world = game->world;
    Player *player = game->player;
    Map *map = world->maps[player->globalY + WORLD_Y_SPAN][player->globalX + WORLD_X_SPAN];

    bool quitFlag = false;
    while (true) {
        if (quitFlag) break;

        // Start graphics
        char *promptOverride = NULL;

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
        while (true) {
            fgets(cmd, CMD_MAX_LENGTH, stdin);
            first = cmd[0];

            // Process the input
            if (first == 'q') {
                // q: Quit game
                quitFlag = true;
                break;

            } else if (first == 'n' || first == 'w' || first == 's' || first == 'e') {
                // nswe: Move along the world in cardinal directions
                MapEntryProps entryProps;
                map = moveInWorldDirection(game, cmd[0], &entryProps);
                setupGameOnMapLoad(game, &entryProps, options);
                if (map != NULL) break;  // NULL only if invalid move

                // Else, it was an invalid move, so let it exit the if body,
                promptOverride = "You reached the map border, try again: ";

            } else if (first == 'f') {
                // f <x> <y>: Fly to a specific map
                int x, y;
                int parsedItems = sscanf(cmd, "f %d %d", &x, &y);
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
                int **distanceField = generateDistanceField(map, player->mapX, player->mapY, HIKER);

                if (cmd[1] == 'h') {
                    printDistanceFieldAlt(distanceField);
                } else {
                    printDistanceField(distanceField);
                }

                promptOverride = "Printed Hiker's distance map. Input command: ";

            } else if (first == 'r') {
                // r: Rival distance map
                int **distanceField = generateDistanceField(map, player->mapX, player->mapY, RIVAL);

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

Map *moveInWorldDirection(GameManager *game, char dir, MapEntryProps *entryProps) {
    int worldSeed = game->world->worldSeed;
    int x = game->player->globalX;
    int y = game->player->globalY;
    int dx = 0;
    int dy = 0;
    int playerSpawnX;
    int playerSpawnY;

    switch (dir) {
        case 'n':
            dy = -1;
            playerSpawnX = hashWithMapCardinalDir(x, y, NORTH, worldSeed);
            playerSpawnY = MAP_HEIGHT - 2;
            break;
        case 's':
            dy = 1;
            playerSpawnX = hashWithMapCardinalDir(x, y, SOUTH, worldSeed);
            playerSpawnY = 1;
            break;
        case 'w':
            dx = -1;
            playerSpawnX = MAP_WIDTH - 2;
            playerSpawnY = hashWithMapCardinalDir(x, y, WEST, worldSeed);
            break;
        case 'e':
            dx = 1;
            playerSpawnX = 1;
            playerSpawnY = hashWithMapCardinalDir(x, y, EAST, worldSeed);
            break;
        default:
            printf("Something is very wrong [core/game.c->moveInWorldDirection()]");
            exit(1);
    }

    Map *newMap = moveToMap(game, x + dx, y + dy, entryProps);
    entryProps->playerSpawnX = playerSpawnX;
    entryProps->playerSpawnY = playerSpawnY;
    return newMap;
}

Map *moveToMap(GameManager *game, int globalX, int globalY, MapEntryProps *entryProps) {
    Map *newMap = getMap(game->world, entryProps, globalX, globalY, true);
    if (newMap != NULL) {
        game->player->globalX = globalX;
        game->player->globalY = globalY;
    }

    return newMap;
}

void setupGameOnMapLoad(GameManager *game, MapEntryProps *entryProps, GameOptions *options) {
    printf(CLEAR_SCREEN);
    Player *player = game->player;
    player->mapX = entryProps->playerSpawnX;
    player->mapY = entryProps->playerSpawnY;
    Map *map = game->world->maps[player->globalY + WORLD_Y_SPAN][player->globalX + WORLD_X_SPAN];
    game->entManager = instantiateEntityManager(game);
    game->time = 0;

    EntityType types[] = {HIKER, RIVAL, PACER, WANDERER, SENTRY, EXPLORER};
    int numTypes = 6;
    for (int i = 0; i < options->numTrainers; i++) {
        EntityType entType;
        Entity *ent = NULL;

        for (int _ = 0; _ < MAX_ITERATIONS && ent == NULL; _++) {
            if (i == 0) entType = HIKER;
            else if (i == 1) entType = RIVAL;
            else entType = types[randomInt(0, numTypes - 1)];

            int x = randomInt(1, MAP_WIDTH - 2);
            int y = randomInt(1, MAP_HEIGHT - 2);

            if (getTerrainCost(map->tileset[y][x].type, entType) == UNCROSSABLE) continue;

            ent = constructEntity(
                game->entManager,
                entType,
                x,
                y
            );
        }

        if (ent == NULL) {
            printf("Failed to place the %d-th trainer, too crowded\n", i);
            return;
        }
    }

}

void printGameState(GameManager *game) {

}

