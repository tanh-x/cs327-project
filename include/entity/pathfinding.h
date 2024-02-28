#ifndef PATHFINDING_H
#define PATHFINDING_H


#include "entities.h"
#include "world/mapbuilder.h"
#include "core/player.h"

#define UNCROSSABLE 8001001
#define UNVISITED (-1)

typedef struct DistanceField {
    EntityType entityType;
    int **map;
} DistanceField;

int getTerrainCost(TileType tileType, EntityType entityType);

DistanceField *generateDistanceField(Map *map, int sourceX, int sourceY, EntityType entityType);

DistanceField *getOrComputeDistanceField(DistanceField *memoized[], EntityType entityType, Map *map, Player *player);

void printDistanceField(DistanceField *distanceField);

void printDistanceFieldAlt(DistanceField *distanceField);

void invalidateMemoization(DistanceField *memoized[]);

void disposeDistanceField(DistanceField *distanceField);

#endif
