#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "graphics/artist.h"
#include "world/mapbuilder.h"
#include "utils/mathematics.h"
#include "world/world.h"

#define CLEAR_SCREEN "\033[2J\033[H"

int main(int argc, char *argv[]) {
    printf("%s", CLEAR_SCREEN);
    bool doColoring = true;
    bool useBadApple = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--nocolor") == 0) doColoring = false;
        else if (strcmp(argv[i], "--badapple") == 0) useBadApple = true;
    }

    char *val = getenv("START");
    long long int invocationStartTime;
    if (val != NULL) invocationStartTime = strtol(val, NULL, 10) % 2190666 + 16270;
    else invocationStartTime = 0;
    struct timespec timeNano;
    timespec_get(&timeNano, TIME_UTC);
    long long int timeSeedMilli = (timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime;

    World world;
    initializeWorld(&world, (int) (timeSeedMilli & 0xffffffffLL));

    // Generate the map
    Map* map = getMap(&world, 0, 0);
    map->mapSeed = timeSeedMilli & 0xffffffff; // NOLINT(*-narrowing-conversions)
    map->globalX = 0;
    map->globalY = 0;
    generateMap(map, useBadApple);

    // Print it to stdout
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(map, mapStr);

    prettyPrint(mapStr, doColoring);

    destroyWorld(&world);
    return 0;
}
