#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "../include/graphics/artist.h"
#include "../include/world/mapbuilder.h"
#include "../include/utils/mathematics.h"

#define CLEAR_SCREEN "\033[2J\033[H"

int main(int argc, char *argv[]) {
    printf("%s", CLEAR_SCREEN);
    bool doColoring;
    bool useBadApple;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--color") == 0) doColoring = true;
        else if (strcmp(argv[i], "--badapple") == 0) useBadApple = true;
    }

    char *val = getenv("START");
    long long int invocationStartTime;
    if (val != NULL) invocationStartTime = strtol(val, NULL, 10) % 2190666 + 16270;
    else invocationStartTime = 0;
    struct timespec timeNano;
    timespec_get(&timeNano, TIME_UTC);
    long long int timeSeedMilli = (timeNano.tv_sec * 1000LL + timeNano.tv_nsec / 1000000LL) - invocationStartTime;


    // Generate the map
    Map map;
    map.mapSeed = timeSeedMilli & 0xffffffff; // NOLINT(*-narrowing-conversions)
    map.globalX = 0;
    map.globalY = 0;
    initializeMap(&map, useBadApple);

    // Print it to stdout
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(&map, mapStr);

    prettyPrint(mapStr, doColoring);

    return 0;
}
