#include "mapbuilder.h"
#include "utils/mathematics.h"

#ifndef WORLD_H
#define WORLD_H

#define WORLD_X_SPAN 200
#define WORLD_Y_SPAN 200

// Coupled with above
#define WORLD_WIDTH 401
#define WORLD_HEIGHT 401

typedef struct {
    Map *maps[WORLD_HEIGHT][WORLD_WIDTH];
    int worldSeed;
} World;

void initializeWorld(World *world, int worldSeed);

Map *getMap(World *world, MapEntryProps *entryProps, int globalX, int globalY, bool generateIfNull);

int globalHashFunction(int x, int y, int worldSeed);

int hashWithMapCardinalDir(int mapX, int mapY, CardinalDir dir, int worldSeed);

void destroyWorld(World *world);

#endif
