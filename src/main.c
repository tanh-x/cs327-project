#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "graphics/artist.h"
#include "world/mapbuilder.h"
#include "world/world.h"
#include "core/game.h"

int main(int argc, char *argv[]) {
    bool doColoring = true;
    bool useBadApple = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--nocolor") == 0) doColoring = false;
        else if (strcmp(argv[i], "--badapple") == 0) useBadApple = true;
    }

    GameOptions options;
    options.doColoring = doColoring;

    char *val = getenv("START");
    long long int invocationStartTime;
    if (val != NULL) invocationStartTime = strtol(val, NULL, 10) % 2190666 + 69820;
    else invocationStartTime = 0;
    struct timespec timeNano;
    timespec_get(&timeNano, TIME_UTC);
    int timeSeedMilli = (int) ((timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime);


    GameManager game;

    Player player;
    player.globalX = 0;
    player.globalY = 0;
    game.player = &player;

    World world;
    initializeWorld(&world, timeSeedMilli);
    game.world = &world;

    // Get and generate the central map
    Map *map = getMap(&world, player.globalX, player.globalY, false);
    // Override seed for the center map
    map->mapSeed = timeSeedMilli; // NOLINT(*-narrowing-conversions)
    generateMap(map, world.worldSeed, useBadApple);

    // Initial print of the map
    printf("%s", CLEAR_SCREEN);
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(map, mapStr);
    prettyPrint(mapStr, options.doColoring);

    // Override gameloop
    if (useBadApple) return 0;

    // Enter game loop
    update(&game, options);
    return 0;
}
