#include <cstdlib>
#include "utils/mathematics.hpp"
#include "world/mapbuilder.hpp"
#include "entity/pathfinding.hpp"
#include "utils/heap.h"


// NOLINTBEGIN
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
//    switch (tileType) {
//        case FLAT:          return 10;
//        case BOULDER:       return UNCROSSABLE;
//        case BORDER:        return UNCROSSABLE;
//        case MOUNTAIN:      return UNCROSSABLE;
//        case GATE:          return 10;
//        case ROAD:          return 10;
//        case BOULDER_ROAD:  return 10;
//        case TALL_GRASS:    return 20;
//        case WATER:         return UNCROSSABLE;
//        case TREE:          return UNCROSSABLE;
//        case POKECENTER:    return 10;
//        case POKEMART:      return 10;
//        case JOULDER:       return UNCROSSABLE;
//        default:            return UNCROSSABLE;
//    }

    // Reduced costs, so the player can more easily outrun NPCs
    switch (tileType) {
        case FLAT:          return 8;
        case BOULDER:       return UNCROSSABLE;
        case BORDER:        return UNCROSSABLE;
        case MOUNTAIN:      return UNCROSSABLE;
        case GATE:          return 5;
        case ROAD:          return 5;
        case BOULDER_ROAD:  return 8;
        case TALL_GRASS:    return 15;
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

int getTerrainCost(TileType tileType, EntityEnum entityType) {
    switch (entityType) {
        case EntityEnum::PLAYER:        return getTerrainCostPlayer(tileType);
        case EntityEnum::HIKER:         return getTerrainCostHiker(tileType);
        case EntityEnum::RIVAL:         return getTerrainCostRival(tileType);
        case EntityEnum::SWIMMER:       return getTerrainCostSwimmer(tileType);
        default:            return getDefaultTerrainCost(tileType);
    }
}
// @formatter:on
// NOLINTEND

typedef struct {
    int tileX;
    int tileY;
    int cost;
} TileNode;

int compareTileNode(const void* tile, const void* other) {
    return ((TileNode*) tile)->cost - ((TileNode*) other)->cost;
}

// Computes the distance field for a given entity type and source coordinates
// Returns the pointer to a DistanceField object
DistanceField* generateDistanceField(Map* map, int sourceX, int sourceY, EntityEnum entityType) {
    auto* field = static_cast<DistanceField*>(malloc(sizeof(DistanceField)));
    field->entityType = entityType;

    // Initialize 2D array of ints
    int** distanceField = static_cast<int**>(malloc(MAP_HEIGHT * sizeof(int*)));
    if (distanceField == nullptr) {
        free(field);
        return nullptr;
    }
    field->map = distanceField;

    // Malloc and initialize the 2nd dimension of the field
    for (int y = 0; y < MAP_HEIGHT; y++) {
        distanceField[y] = static_cast<int*>(malloc(MAP_WIDTH * sizeof(int)));
        if (distanceField[y] == nullptr) {
            for (int i = 0; i < y; i++) free(distanceField[i]);
            free(field);
            free(distanceField);
            return nullptr;
        }
        // Set to unvisited
        for (int x = 0; x < MAP_WIDTH; x++) distanceField[y][x] = UNVISITED;
    }
    // Now, distanceField is a 2D array filled with the same map (`UNCROSSABLE`)

    // Initialize a new heap
    heap_t heap;
    heap_init(&heap, compareTileNode, free);

    // Malloc and add in the source node as the first element on the heap
    auto* source = static_cast<TileNode*>(malloc(sizeof(TileNode)));
    source->tileX = sourceX;
    source->tileY = sourceY;
    source->cost = getTerrainCost(map->tileset[sourceY][sourceX].type, entityType);
    distanceField[sourceY][sourceX] = source->cost;
    heap_insert(&heap, source);

    // Check if the player is standing on an inaccessible tile to the entity, if so, then give up
    if (source->cost == UNCROSSABLE) {
        heap_delete(&heap);
        return field;
    }

    // Start carrying out Dijkstra's algorithm
    TileNode* u;
    while ((u = static_cast<TileNode*>(heap_remove_min(&heap)))) {
        for (int vx = max(u->tileX - 1, 0); vx <= min(u->tileX + 1, MAP_WIDTH - 1); vx++) {
            for (int vy = max(u->tileY - 1, 0); vy <= min(u->tileY + 1, MAP_HEIGHT - 1); vy++) {
                // Check if the neighboring tile is uncrossable
                int tileCost = getTerrainCost(map->tileset[vy][vx].type, entityType);
                if (tileCost == UNCROSSABLE) continue;

                // Check if it's cheaper than the previous cost
                int newCost = u->cost + tileCost;
                if (distanceField[vy][vx] != UNVISITED && newCost >= distanceField[vy][vx]) continue;

                distanceField[vy][vx] = newCost;
                auto* v = static_cast<TileNode*>(malloc(sizeof(TileNode)));
                v->tileX = vx;
                v->tileY = vy;
                v->cost = newCost;
                heap_insert(&heap, v);
            }
        }
        free(u);
    }

    heap_delete(&heap);
    return field;
}

// Checks the given memoized array for a matching distance field, if none is found, compute and put it in.
// Returns the pointer to a DistanceField object
DistanceField* getOrComputeDistanceField(DistanceField* memoized[], EntityEnum entityType, Map* map, Player* player) {
    int i = 0;
    for (;; i++) {
        if (i >= DISTANCE_FIELD_MEMOIZATION_SIZE) {
            // Ran out of space on the memoization array, so invalidate everything to make space.
            invalidateMemoization(memoized);
            i = 0;
            break;
        }
        // Grab the pointer at index i
        DistanceField* field = memoized[i];
        // NULL means that it's all NULLs past this index, so we can fill in a new field here
        if (field == nullptr) break;
        // Otherwise, if we found a valid field, return it
        if (field->entityType == entityType) return memoized[i];
    }
    // If we got here, it means we haven't computed this distance field yet
    DistanceField* newField = generateDistanceField(map, player->mapX, player->mapY, entityType);
    memoized[i] = newField;
    return newField;
}

// Clears and frees the memoization array
void invalidateMemoization(DistanceField* memoized[]) {
    if (memoized == nullptr) return;
    for (int i = 0; i < DISTANCE_FIELD_MEMOIZATION_SIZE; i++) {
        DistanceField* field = memoized[i];
        if (field != nullptr) {
            disposeDistanceField(field);
            memoized[i] = nullptr;
        }
    }
}

// Frees a distance field
void disposeDistanceField(DistanceField* distanceField) {
    int** map = distanceField->map;
    for (int row = 0; row < MAP_HEIGHT; row++) {
        free(map[row]);
    }
    free(map);
    free(distanceField);
}
