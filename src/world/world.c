#include <stdbool.h>
#include "../../include/world/world.h"

#define b 0xDEFCBA9
#define p1 963676709
#define p2 298482817
#define p3 709348097

void initializeWorld(World *world, int worldSeed) {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            world->filled[j][i] = false;
        }
    }
    world->worldSeed = worldSeed;
}

bool getOrInitialize(World *world, Map *emptyMap, int globalX, int globalY) {

    int i = globalX + WORLD_X_SPAN;
    int j = globalY + WORLD_Y_SPAN;

    if (!world->filled[j][i]) {
        emptyMap = world->maps[j][i];
        emptyMap->globalX = globalX;
        emptyMap->globalY = globalY;
        emptyMap->mapSeed = globalHashFunction(globalX, globalY, world->worldSeed + p3);
    } else {
        emptyMap = world->maps[j][i];
        return true;
    }
}

int globalHashFunction(int globalX, int globalY, int worldSeed) {
    return (((globalX * worldSeed) ^ (globalY ^ b)) * p3 + ((globalX * p1) ^ (globalY * p2))) & 0xffffff;
}
