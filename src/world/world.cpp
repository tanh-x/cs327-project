#include <cstdlib>
#include <cmath>
#include "world/world.hpp"
#include "core/game.hpp"

#define b 0xDEFCBA9
#define p1 963676709
#define p2 298482817
#define p3 709348097
#define p4 485820427

//#define OVERGROWTH_FACTOR 12.6277f

void initializeWorld(World* world, int worldSeed) {
    srand(worldSeed);
    for (int y = 0; y < WORLD_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            world->maps[y][x] = nullptr;

            auto eccentricity = static_cast<uint8_t>(rand());
            auto dist = float(abs(x - WORLD_X_SPAN) + abs(y - WORLD_Y_SPAN));

            float overgrowth = OVERGROWTH_FACTOR * sqrtf(dist) * randomFloat(0.25f, 1.75f);
            float menace = getMenaceLevel(x - WORLD_X_SPAN, y - WORLD_Y_SPAN, eccentricity, worldSeed);
            float wilderness = randomFloat(WILDERNESS_LEVEL_LOWER_BOUND, WILDERNESS_LEVEL_UPPER_BOUND)
                               + overgrowth / OVERGROWTH_WILDERNESS_MULTIPLIER;

            world->eccentricity[y][x] = eccentricity;
            world->overgrowthLevel[y][x] = overgrowth;
            world->menaceLevel[y][x] = menace;
            world->wildernessLevel[y][x] = wilderness;
        }
    }
    world->worldSeed = worldSeed;
}

Map* getMap(World* world, int globalX, int globalY, bool generateIfNull) {
    int i = globalX + WORLD_X_SPAN;
    int j = globalY + WORLD_Y_SPAN;
    if (i < 0 || i >= WORLD_WIDTH || j < 0 || j >= WORLD_HEIGHT) return nullptr;

    if (world->maps[j][i] == nullptr) {
        Map* newMap = new Map(globalX, globalY, world->worldSeed + p4);
        world->maps[j][i] = newMap;
        if (generateIfNull) newMap->generateTerrain(world->worldSeed);
    }

    return world->maps[j][i];
}

// Used to synchronize gate position between maps. See README for more info
int globalHashFunction(int x, int y, int worldSeed) {
    return ((((x * worldSeed) ^ (y ^ b)) * p3 + ((x * p1) ^ (y * p2))) & 0xffffff) + worldSeed;
}

// Wrapper around the hash function for more convenient usage
int hashWithMapCardinalDir(int mapX, int mapY, CardinalDir dir, int worldSeed) {
    switch (dir) {
        case NORTH:
            return positiveMod(
                globalHashFunction(2 * mapX, 2 * mapY - 1, worldSeed),
                MAP_WIDTH - 4 * GATE_PADDING
            ) + 2 * GATE_PADDING;
        case SOUTH:
            return positiveMod(
                globalHashFunction(2 * mapX, 2 * mapY + 1, worldSeed),
                MAP_WIDTH - 4 * GATE_PADDING
            ) + 2 * GATE_PADDING;
        case WEST:
            return positiveMod(
                globalHashFunction(2 * mapX - 1, 2 * mapY, worldSeed),
                MAP_HEIGHT - 2 * GATE_PADDING
            ) + GATE_PADDING;
        case EAST:
            return positiveMod(
                globalHashFunction(2 * mapX + 1, 2 * mapY, worldSeed),
                MAP_HEIGHT - 2 * GATE_PADDING
            ) + GATE_PADDING;
    }
    // TODO: test, remove later
    exit(1);
}

void disposeWorld(World* world) {
    for (auto &i: world->maps) {
        for (auto &map: i) {
            if (map == nullptr) continue;

            free(map->entityManager);
            map->entityManager = nullptr;
            free(map);
            map = nullptr;
        }
    }
}

int getEccentricity(int globalX, int globalY) {
    return GAME.world->eccentricity[globalY + WORLD_Y_SPAN][globalX + WORLD_X_SPAN];
}
