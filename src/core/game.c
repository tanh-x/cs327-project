#include <stdio.h>
#include "core/game.h"
#include "graphics/artist.h"

void update(GameManager *game, GameOptions options) {
    World *world = game->world;
    Player *player = game->player;
    Map *map = world->maps[player->globalY + WORLD_Y_SPAN][player->globalX + WORLD_X_SPAN];

    bool quitFlag = false;
    while (true) {
        if (quitFlag) break;

        // Start graphics
        printf("%s", CLEAR_SCREEN);

        // Print the map
        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
        worldToString(map, mapStr);
        prettyPrint(mapStr, options.doColoring);

        // First line
        printf("Map position: (%d, %d)", player->globalX, player->globalY);
        printf("\n");

        // Second line

        // Third line
        char cmd[CMD_MAX_LENGTH];
        char first;
        printf("Input command: ");
        while (true) {
            fgets(cmd, CMD_MAX_LENGTH, stdin);
            first = cmd[0];

            // Process the input
            if (first == 'q') {
                quitFlag = true;
                break;
            } else if (first == 'n' || first == 'w' || first == 's' || first == 'e') {
                map = moveInWorldDirection(game, cmd[0]);
                if (map != NULL) break;  // NULL if invalid move
                // Else, let it exit the if body
            } else if (first == 'f') {
                int x, y;
                int parsedItems = sscanf(cmd, "f %d %d", &x, &y);
                if (parsedItems == 2) {
                    map = moveToMap(game, x, y);
                    if (map != NULL) break;  // NULL if invalid move
                    // Else, let it exit the if body
                }
            }
            // If we got here, it was an invalid command
            printf("\033[A\r\033[K");
            printf("Invalid command, try again: ");
        }
    }
}

Map *moveInWorldDirection(GameManager *game, char dir) {
    int dx = dir == 'e' ? 1 : (dir == 'w' ? -1 : 0);
    int dy = dir == 's' ? 1 : (dir == 'n' ? -1 : 0);
    return moveToMap(game, game->player->globalX + dx, game->player->globalY + dy);
}

Map *moveToMap(GameManager *game, int globalX, int globalY) {
    Map *newMap = getMap(game->world, globalX, globalY, true);
    if (newMap != NULL) {
        game->player->globalX = globalX;
        game->player->globalY = globalY;
    }

    return newMap;
}

void printGameState(GameManager *game) {

}

