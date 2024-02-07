#include <stdio.h>
#include "core/game.h"
#include "graphics/artist.h"

#define CLEAR_SCREEN "\033[2J\033[H"

void update(GameManager *game, GameOptions options) {
    World world = *(game->world);
    Player player = *(game->player);
    Map map = *(world.maps[player.globalY + WORLD_Y_SPAN][player.globalX + WORLD_X_SPAN]);

    bool quitFlag = false;
    while (true) {
        if (quitFlag) break;

        // Start graphics
        printf("%s", CLEAR_SCREEN);

        // Print the map
        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
        worldToString(&map, mapStr);
        prettyPrint(mapStr, options.doColoring);

        // First line

        // Second line


        // Third line
        char cmd[CMD_MAX_LENGTH];
        char first = 0;
        printf("$ ");
        while (true) {
            scanf("%s", cmd);
            first = cmd[0];

            // Process the input
            if (first == 'q') {
                quitFlag = true;
                break;
            } else if (first == 'n' || first == 'w' || first == 's' || first == 'e') {
                map = *moveInWorldDirection(game, cmd[0]);
                break;
            } else if (first == 'f') {
                int x, y;
                int parsedItems = sscanf(cmd, "f %d %d", &x, &y);
                if (parsedItems == 2) {
                    map = *moveToMap(game, x, y);
                    break;
                }
            }
            // If we got here, it was an invalid command
            printf("\r\033[K");
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
    game->player->globalX = globalX;
    game->player->globalY = globalY;

    return getMap(game->world, globalX, globalY, true);
}

void printGameState(GameManager *game) {

}

