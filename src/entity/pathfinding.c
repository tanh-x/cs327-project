#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-branch-clone"

#include <stdlib.h>
#include <stdio.h>
#include "world/mapbuilder.h"
#include "entity/entities.h"
#include "entity/pathfinding.h"
#include "utils/heap.h"
#include "utils/mathematics.h"

// @formatter:off
int getDefaultTerrainCost(TileType tileType) {
    switch (tileType) {
        case FLAT:          return 10;
        case BOULDER:       return UNCROSSABLE;
        case BORDER:        return UNCROSSABLE;
        case MOUNTAIN:      return UNCROSSABLE;
        case GATE:          return UNCROSSABLE;
        case ROAD:          return 10;
        case BOULDER_ROAD:  return 10;
        case TALL_GRASS:    return 20;
        case WATER:         return UNCROSSABLE;
        case TREE:          return UNCROSSABLE;
        case POKECENTER:    return 50;
        case POKEMART:      return 50;
        case JOULDER:       return UNCROSSABLE;
        default:            return UNCROSSABLE;
    }
}

int getTerrainCostPlayer(TileType tileType) {
    switch (tileType) {
        case FLAT:          return 10;
        case BOULDER:       return UNCROSSABLE;
        case BORDER:        return UNCROSSABLE;
        case MOUNTAIN:      return UNCROSSABLE;
        case GATE:          return 10;
        case ROAD:          return 10;
        case BOULDER_ROAD:  return 10;
        case TALL_GRASS:    return 20;
        case WATER:         return UNCROSSABLE;
        case TREE:          return UNCROSSABLE;
        case POKECENTER:    return 10;
        case POKEMART:      return 10;
        case JOULDER:       return UNCROSSABLE;
        default:            return UNCROSSABLE;
    }
}

int getTerrainCostHiker(TileType tileType) {
    switch (tileType) {
        case FLAT:          return 10;
        case BOULDER:       return 15;
        case BORDER:        return UNCROSSABLE;
        case MOUNTAIN:      return 15;
        case GATE:          return UNCROSSABLE;
        case ROAD:          return 10;
        case BOULDER_ROAD:  return 10;
        case TALL_GRASS:    return 15;
        case WATER:         return UNCROSSABLE;
        case TREE:          return UNCROSSABLE;
        case POKECENTER:    return 50;
        case POKEMART:      return 50;
        case JOULDER:       return UNCROSSABLE;
        default:            return UNCROSSABLE;
    }
}

int getTerrainCostRival(TileType tileType) {
    switch (tileType) {
        case FLAT:          return 10;
        case BOULDER:       return UNCROSSABLE;
        case BORDER:        return UNCROSSABLE;
        case MOUNTAIN:      return UNCROSSABLE;
        case GATE:          return UNCROSSABLE;
        case ROAD:          return 10;
        case BOULDER_ROAD:  return 10;
        case TALL_GRASS:    return 20;
        case WATER:         return UNCROSSABLE;
        case TREE:          return UNCROSSABLE;
        case POKECENTER:    return 50;
        case POKEMART:      return 50;
        case JOULDER:       return UNCROSSABLE;
        default:            return UNCROSSABLE;
    }
}

int getTerrainCostSwimmer(TileType tileType) {
    switch (tileType) {
        case WATER:         return 7;
        default:            return UNCROSSABLE;
    }
}


int getTerrainCost(TileType tileType, EntityType entityType) {
    switch (entityType) {
        case PLAYER:        return getTerrainCostPlayer(tileType);
        case HIKER:         return getTerrainCostHiker(tileType);
        case RIVAL:         return getTerrainCostRival(tileType);
        case SWIMMER:       return getTerrainCostSwimmer(tileType);
        default:            return getDefaultTerrainCost(tileType);
    }
}
// @formatter:on

typedef struct {
    int tileX;
    int tileY;
    int cost;
} TileNode;

int compareTileNode(const void *this, const void *other) {
    return ((TileNode *) this)->cost - ((TileNode *) other)->cost;
}

DistanceField *generateDistanceField(Map *map, int sourceX, int sourceY, EntityType entityType) {
    DistanceField *field = malloc(sizeof(DistanceField));
    field->entityType = entityType;

    // Initialize 2D array of ints
    int **distanceField = malloc(MAP_HEIGHT * sizeof(int *));
    if (distanceField == NULL) {
        free(field);
        return NULL;
    }
    field->map = distanceField;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        distanceField[y] = malloc(MAP_WIDTH * sizeof(int));
        if (distanceField[y] == NULL) {
            for (int i = 0; i < y; i++) free(distanceField[i]);
            free(field);
            free(distanceField);
            return NULL;
        }
        for (int x = 0; x < MAP_WIDTH; x++) distanceField[y][x] = UNVISITED;
    }

    // Now, distanceField is a 2D array filled with the same map (`UNCROSSABLE`)

    heap_t heap;
    heap_init(&heap, compareTileNode, NULL);

    TileNode *source = malloc(sizeof(TileNode));
    source->tileX = sourceX;
    source->tileY = sourceY;
    source->cost = getTerrainCost(map->tileset[sourceY][sourceX].type, entityType);

    if (source->cost == UNCROSSABLE) return field;

    distanceField[sourceY][sourceX] = source->cost;
    heap_insert(&heap, source);

    TileNode *u;
    while ((u = heap_remove_min(&heap))) {
        for (int vx = max(u->tileX - 1, 0); vx <= min(u->tileX + 1, MAP_WIDTH - 1); vx++) {
            for (int vy = max(u->tileY - 1, 0); vy <= min(u->tileY + 1, MAP_HEIGHT - 1); vy++) {
                int tileCost = getTerrainCost(map->tileset[vy][vx].type, entityType);
                if (tileCost == UNCROSSABLE) continue;

                int newCost = u->cost + tileCost;
                if (distanceField[vy][vx] != UNVISITED && newCost >= distanceField[vy][vx]) continue;

                distanceField[vy][vx] = newCost;
                TileNode *v = malloc(sizeof(TileNode));
                v->tileX = vx;
                v->tileY = vy;
                v->cost = newCost;
                heap_insert(&heap, v);
            }
        }
    }

    return field;
}

void printDistanceField(DistanceField *distanceField) {
    int** map = distanceField->map;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == UNVISITED) printf("   ");
            else printf("%02d ", map[y][x] % 100);
        }
        printf("\n");
    }
}

void printDistanceFieldAlt(DistanceField *distanceField) {
    int** map = distanceField->map;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == UNVISITED) printf("   ");
            else printf("%02d ", (map[y][x] / 10) % 100);
        }
        printf("\n");
    }
}


#pragma clang diagnostic pop