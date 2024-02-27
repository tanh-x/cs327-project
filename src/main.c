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
    bool doBadApple = false;
    int numTrainers = 10;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        char *flag = argv[i];
        if (strcmp(flag, "--nocolor") == 0) doColoring = false;
        else if (strcmp(flag, "--badapple") == 0) doBadApple = true;
        else if (strcmp(flag, "--numtrainers") == 0) numTrainers = (int) strtol(argv[i + 1], NULL, 10);
    }

    // Initialize options to reflect the parsed arguments
    GameOptions options;
    options.doColoring = doColoring;
    options.numTrainers = max(0, numTrainers);

    // Get millisecond seed
    char *val = getenv("START");
    long long int invocationStartTime;
    if (val != NULL) invocationStartTime = strtol(val, NULL, 10) % 2190666 + 69820;
    else invocationStartTime = 0;
    struct timespec timeNano;
    timespec_get(&timeNano, TIME_UTC);
    int timeSeedMilli = (int) ((timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime);

    // Initial set up
    GameManager game;

    Player player;
    player.globalX = 0;
    player.globalY = 0;
    game.player = &player;

    World world;
    initializeWorld(&world, timeSeedMilli);
    game.world = &world;

    // Get and generate the central map
    MapEntryProps entryProps;
    Map *map = getMap(&world, &entryProps, player.globalX, player.globalY, false);
    map->isSpawnMap = true;
    // Override seed for the center map
    map->mapSeed = timeSeedMilli; // NOLINT(*-narrowing-conversions)
    generateMap(map, &entryProps, world.worldSeed, doBadApple);
    setupGameOnMapLoad(&game, &entryProps, &options);

    // Override game loop if badapple
    if (doBadApple) {
        printf(CLEAR_SCREEN);
        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
        worldToString(&game, mapStr);
        prettyPrint(mapStr, options.doColoring);
        return 0;
    }

    // Enter game loop
    update(&game, &options);
    return 0;
}
