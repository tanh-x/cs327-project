#ifndef WORLD_H
#define WORLD_H

#include "mapbuilder.hpp"
#include "utils/mathematics.hpp"
#include "core/constants/primary.hpp"

#define OVERGROWTH_FACTOR 23.99263f
#define WILDERNESS_LEVEL_LOWER_BOUND 0.0175f
#define WILDERNESS_LEVEL_UPPER_BOUND 0.0645f
#define OVERGROWTH_WILDERNESS_MULTIPLIER 2360.0f

typedef struct {
    Map* maps[WORLD_HEIGHT][WORLD_WIDTH];

    // Mystery number
    uint8_t eccentricity[WORLD_HEIGHT][WORLD_WIDTH];
    float overgrowthLevel[WORLD_HEIGHT][WORLD_WIDTH];
    float menaceLevel[WORLD_HEIGHT][WORLD_WIDTH];
    float wildernessLevel[WORLD_HEIGHT][WORLD_WIDTH];

    Map* current;
    int worldSeed;
} World;

void initializeWorld(World* world, int worldSeed);

Map* getMap(World* world, int globalX, int globalY, bool generateIfNull);

// Used to synchronize gate position between maps. See README for more info
int globalHashFunction(int x, int y, int worldSeed);

// Wrapper around the hash function for more convenient usage
int hashWithMapCardinalDir(int mapX, int mapY, CardinalDir dir, int worldSeed);

int getEccentricity(int globalX, int globalY);

void disposeWorld(World* world);

#endif
