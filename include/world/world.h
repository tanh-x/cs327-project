#include "mapbuilder.h"

#ifndef WORLD_H
#define WORLD_H

#define WORLD_WIDTH 401
#define WORLD_HEIGHT 401

typedef struct {
    Map maps[WORLD_HEIGHT][WORLD_WIDTH];
    int worldSeed;
} World;

int globalHashFunction(int globalX, int globalY, int worldSeed);

#endif
