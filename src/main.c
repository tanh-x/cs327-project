#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "graphics/artist.h"
#include "world/mapbuilder.h"
#include "world/world.h"
#include "core/game.h"
#include "graphics/renderer.h"

int main(int argc, char* argv[]) {
    // Default game option arguments
    bool doColoring = true;
    bool doBadApple = false;
    int numTrainers = 10;
    int frameTimeMicros = 1000000 / 5000;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        char* flag = argv[i];
        if (strcmp(flag, "--nocolor") == 0) doColoring = false;
        else if (strcmp(flag, "--badapple") == 0) doBadApple = true;
        else if (strcmp(flag, "--numtrainers") == 0) numTrainers = (int) strtol(argv[i + 1], NULL, 10);
        else if (strcmp(flag, "--frametime") == 0) frameTimeMicros = (int) strtol(argv[i + 1], NULL, 10);
    }

    // Initialize options to reflect the parsed arguments
    GameOptions options;
    options.doColoring = doColoring;
    options.numTrainers = max(0, numTrainers);
    options.frameTimeMicros = frameTimeMicros;

    // Get millisecond seed
    char* val = getenv("START");
    long long int invocationStartTime;
    if (val != NULL) invocationStartTime = strtol(val, NULL, 10) % 2190666 + 69820;
    else invocationStartTime = 0;
    struct timespec timeNano;
    timespec_get(&timeNano, TIME_UTC);
    int timeSeedMilli = (int) ((timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime);

    // Initialize ncurses
    initializeRenderer(options.doColoring);

    // TODO: Main menu?

    // Initial set up
    GameManager game;
    game.entManager = NULL;
    game.quit_game = false;

    // Create a player struct
    Player player;
    player.globalX = 0;
    player.globalY = 0;
    game.player = &player;

    // Initialize a world with all empty maps
    World world;
    initializeWorld(&world, timeSeedMilli);
    game.world = &world;

    // Get and generate the central map
    MapEntryProps entryProps;
    Map* map = getMap(&world, &entryProps, player.globalX, player.globalY, false);

    // Set the current map to this map, allowing for convenient future reference
    world.current = map;

    // Initialize the memoized distance fields array
    for (int i = 0; i < DISTANCE_FIELD_MEMOIZATION_SIZE; i++) map->memoizedDistanceFields[i] = NULL;

    // spawnMap will allow for special case world generation
    map->isSpawnMap = true;

    // Override seed for the center map
    map->mapSeed = timeSeedMilli; // NOLINT(*-narrowing-conversions)

    // Generate the first map outside the game loop, prevents it from regenerating in the game loop.
    generateMap(map, &entryProps, world.worldSeed, doBadApple);

    // Also manually call the setup function on the spawn map instead of doing it in the game loop
    setupGameOnMapLoad(&game, &entryProps, &options);

    // Override game loop if using bad apple, DON'T PORT TO NCURSES!
    if (doBadApple) {
        printf(CLEAR_SCREEN);
        char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
        worldToString(&game, mapStr);
        prettyPrint(mapStr, options.doColoring);
        return 0;
    }

    // Enter game loop
    update(&game, &options);

    // Clean up
    disposeWorld(&world);
    disposeEntityManager(game.entManager);
    free(map);
    cleanUpRenderer();
    return 0;
}
