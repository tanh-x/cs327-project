#include <time.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "world/world.h"

#define b 0xDEFCBA9
#define p1 963676709
#define p2 298482817
#define p3 709348097
#define p4 485820427

#define OVERGROWTH_FACTOR 12.6277f

void initializeWorld(World *world, int worldSeed) {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            world->maps[j][i] = NULL;
        }
    }
    world->worldSeed = worldSeed;
}

Map *getMap(World *world, MapEntryProps *entryProps, int globalX, int globalY, bool generateIfNull) {
    int i = globalX + WORLD_X_SPAN;
    int j = globalY + WORLD_Y_SPAN;
    if (i < 0 || i >= WORLD_WIDTH || j < 0 || j >= WORLD_HEIGHT) return NULL;

    if (world->maps[j][i] == NULL) {
        Map *newMap = (Map *) malloc(sizeof(Map));
        newMap->globalX = globalX;
        newMap->globalY = globalY;
        newMap->mapSeed = globalHashFunction(globalX, globalY, world->worldSeed + p4);
        newMap->overgrowth = OVERGROWTH_FACTOR * sqrt(abs(globalX) + abs(globalY));
        if (generateIfNull) generateMap(newMap, entryProps, world->worldSeed, false);
        world->maps[j][i] = newMap;
    }

    return world->maps[j][i];
}

int globalHashFunction(int x, int y, int worldSeed) {
    return ((((x * worldSeed) ^ (y ^ b)) * p3 + ((x * p1) ^ (y * p2))) & 0xffffff) + worldSeed;
}

int hashWithMapCardinalDir(int mapX, int mapY, CardinalDir dir, int worldSeed) {
    switch (dir) {
        case NORTH:
            return positiveMod(
                globalHashFunction(2 * mapX, 2 * mapY - 1, worldSeed),
                MAP_WIDTH - 4 * GATE_PADDING
            ) + 2 * GATE_PADDING;
        case SOUTH:
            return positiveMod(
                globalHashFunction(2 * mapX, 2 * mapY + 1, worldSeed),
                MAP_WIDTH - 4 * GATE_PADDING
            ) + 2 * GATE_PADDING;
        case WEST:
            return positiveMod(
                globalHashFunction(2 * mapX - 1, 2 * mapY, worldSeed),
                MAP_HEIGHT - 2 * GATE_PADDING
            ) + GATE_PADDING;
        case EAST:
            return positiveMod(
                globalHashFunction(2 * mapX + 1, 2 * mapY, worldSeed),
                MAP_HEIGHT - 2 * GATE_PADDING
            ) + GATE_PADDING;
    }
    // TODO: test, remove later
    exit(1);
}

void disposeWorld(World *world) {
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_WIDTH; j++) {
            if (world->maps[i][j] != NULL) {
                free(world->maps[i][j]);
                world->maps[i][j] = NULL;
            }
        }
    }
}