#ifndef WORLD_H
#define WORLD_H

#include "mapbuilder.h"
#include "utils/mathematics.h"
#include "core/constants.h"

typedef struct {
    Map *maps[WORLD_HEIGHT][WORLD_WIDTH];
    Map *currentMap;
    int worldSeed;
} World;

void initializeWorld(World *world, int worldSeed);

Map *getMap(World *world, MapEntryProps *entryProps, int globalX, int globalY, bool generateIfNull);

int globalHashFunction(int x, int y, int worldSeed);

int hashWithMapCardinalDir(int mapX, int mapY, CardinalDir dir, int worldSeed);

void disposeWorld(World *world);

#endif
