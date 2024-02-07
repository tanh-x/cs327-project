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

Map *getMap(World *world, int globalX, int globalY, bool generateIfNull) {
    int i = globalX + WORLD_X_SPAN;
    int j = globalY + WORLD_Y_SPAN;
    if (i < 0 || i >= WORLD_WIDTH || j < 0 || j >= WORLD_HEIGHT) return NULL;

    if (world->maps[j][i] == NULL) {
        Map *newMap = (Map *) malloc(sizeof(Map));
        newMap->globalX = globalX;
        newMap->globalY = globalY;
        newMap->mapSeed = globalHashFunction(globalX, globalY, world->worldSeed + p4);
        newMap->overgrowth = OVERGROWTH_FACTOR * sqrt(abs(globalX) + abs(globalY));
        if (generateIfNull) generateMap(newMap, world->worldSeed, false);
        world->maps[j][i] = newMap;
    }

    return world->maps[j][i];
}

int globalHashFunction(int globalX, int globalY, int worldSeed) {
    return ((((globalX * worldSeed) ^ (globalY ^ b)) * p3 + ((globalX * p1) ^ (globalY * p2))) & 0xffffff) + worldSeed;
}

void destroyWorld(World *world) {
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_WIDTH; j++) {
            if (world->maps[i][j] != NULL) {
                free(world->maps[i][j]);
                world->maps[i][j] = NULL;
            }
        }
    }
}