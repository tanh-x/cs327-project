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
    int worldSeed;
} World;

void initializeWorld(World *world, int worldSeed);

Map *getMap(World *world, int globalX, int globalY, bool generateIfNull);

int globalHashFunction(int globalX, int globalY, int worldSeed);

void destroyWorld(World *world);

#endif
