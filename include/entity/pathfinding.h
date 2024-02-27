#include "../world/mapbuilder.h"
#include "entities.h"

#ifndef PATHFINDING_H
#define PATHFINDING_H

#define UNCROSSABLE 8001001
#define UNVISITED (-1)

typedef struct DistanceField {
    EntityType entityType;
    int** map;
} DistanceField;

int getTerrainCost(TileType tileType, EntityType entityType);

DistanceField *generateDistanceField(Map* map, int sourceX, int sourceY, EntityType entityType);

void printDistanceField(DistanceField *distanceField);

void printDistanceFieldAlt(DistanceField *distanceField);

#endif
