#include "mapbuilder.h"

#ifndef WORLD_H
#define WORLD_H

#define WORLD_X_SPAN 200
#define WORLD_Y_SPAN 100

// Coupled with above
#define WORLD_WIDTH 401
#define WORLD_HEIGHT 201

typedef struct {
    Map *maps[WORLD_HEIGHT][WORLD_WIDTH];
    bool filled[WORLD_HEIGHT][WORLD_WIDTH];
    int worldSeed;
} World;

void initializeWorld(World *world, int worldSeed);

bool getOrInitialize(World *world, Map *emptyMap, int globalX, int globalY);

int globalHashFunction(int globalX, int globalY, int worldSeed);

#endif
