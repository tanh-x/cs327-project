#include "mapbuilder.h"
#include "entity/entities.h"

#ifndef PATHFINDING_H
#define PATHFINDING_H

#define UNCROSSABLE 8001001
#define UNVISITED (-1)

//int getTerrainCost(TileType tileType, EntityType entityType);
int getTerrainCost(TileType tileType, EntityType entityType);

int** generateDistanceField(Map* map, int sourceX, int sourceY, EntityType entityType);

void printDistanceField(int** distanceField);

void printDistanceFieldAlt(int** distanceField);

#endif
