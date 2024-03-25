#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "entities.hpp"
#include "world/mapbuilder.hpp"
#include "core/player.hpp"

// A distance field holds a 2D array of integers as well as which entity type it belonged to
typedef struct DistanceField {
    EntityType entityType;
    int** map;
} DistanceField;

int getTerrainCost(TileType tileType, EntityType entityType);

// Computes the distance field for a given entity type and source coordinates
// Returns the pointer to a DistanceField object
DistanceField* generateDistanceField(Map* map, int sourceX, int sourceY, EntityType entityType);

// Checks the given memoized array for a matching distance field, if none is found, compute and put it in.
// Returns the pointer to a DistanceField object
DistanceField* getOrComputeDistanceField(DistanceField* memoized[], EntityType entityType, Map* map, Player* player);

// Clears and frees the memoization array
void invalidateMemoization(DistanceField* memoized[]);

// Frees a distance field
void disposeDistanceField(DistanceField* distanceField);

#endif
