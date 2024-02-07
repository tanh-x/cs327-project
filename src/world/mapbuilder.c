#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "world/mapbuilder.h"
#include "utils/mathematics.h"
#include "graphics/parse_frame.h"
#include "utils/voronoi_noise.h"
#include "world/world.h"

#define MAX_FRAME_COUNT 6572
#define MAX_ITER_MULTIPLIER 8

#define PADDING 2
#define GATE_PADDING 7
#define FIRST_PASS_NUM_TYPES 4

#define NOISE_DENSITY 16
#define NOISE_SCALE 1.025f
#define NOISE_LACUNARITY 9.3242f

#define SCATTER_BOULDER_COUNT 19
#define SCATTER_BOULDER_VARIABILITY 5
#define SCATTER_BOULDER_ON_WATER_PROBABILITY 0.1277f
#define SCATTER_BOULDER_MAX_RADIUS 2.13f

#define DISTORTION_EXP_PROBABILITY 0.7563f
#define DISTORTION_ITERATIONS_BASELINE 80    // More iterations = more overgrown terrain
#define DISTORTION_PADDING 2
#define DISTORTION_KERNEL_RADIUS_SQ 4.85f
#define DISTORTION_ENERGY 3.1416f

#define BLEND_BOULDER_SPREAD_PROBABILITY 0.2192f
#define BLEND_BOULDER_EROSION_PROBABILITY 0.2111f
#define BLEND_BOULDER_WATER_EROSION_PROBABILITY 0.1511f
#define BLEND_SPROUTING_PROBABILITY 0.3022f
#define BLEND_TALL_GRASS_GROWTH_FACTOR 0.7121f
#define BLEND_TREE_SPREAD_PROBABILITY 0.1627f
#define BLEND_TREE_SPREAD_ENVIRONMENTAL_BONUS 1.2621f
#define BLEND_FLAT_SPREAD_PROBABILITY 0.62f

#define ROAD_MOMENTUM_COEFF 0.7621f
#define ROAD_DRIFT_FACTOR 0.121f
#define ROAD_KICK_FACTOR 3.12f
#define ROAD_TARGET_ATTRACTION_FACTOR 2.2572f

#define VORONOI_POINTS_BASE_SEED 21679733

void applyBiomeBlending(Map *map, int tx, int ty, TileType biome) {
    for (int cx = -2; cx <= 2; cx++) {
        for (int cy = -1; cy <= 1; cy++) {
            if ((float) (cx * cx + cy * cy) > DISTORTION_KERNEL_RADIUS_SQ) continue;
            TileType type = map->tileset[ty + cy][tx + cx].type;
            if (type == BOULDER && proba() < BLEND_BOULDER_EROSION_PROBABILITY) continue;

            TileType spread = type;
            if (biome == BOULDER) {
                if ((type == TALL_GRASS || type == WATER)
                    && proba() < BLEND_SPROUTING_PROBABILITY)
                    spread = TREE;
                else if (proba() < BLEND_BOULDER_SPREAD_PROBABILITY) spread = BOULDER;

            } else if (biome == WATER) {
                if (type == BOULDER && proba() < BLEND_BOULDER_WATER_EROSION_PROBABILITY) spread = WATER;

            } else if (biome == TREE) {
                float p = proba();
                if (type == TALL_GRASS || type == FLAT) p /= BLEND_TREE_SPREAD_ENVIRONMENTAL_BONUS;
                if (p < BLEND_TREE_SPREAD_PROBABILITY) spread = TREE;
                else spread = TALL_GRASS;

            } else if (biome == TALL_GRASS) {
                if ((type == TALL_GRASS || type == WATER) &&
                    proba() < BLEND_SPROUTING_PROBABILITY * 0.4)
                    spread = TREE;
                else if (type == WATER || type == BOULDER) spread = TALL_GRASS;
                else if (type == FLAT && proba() < BLEND_TALL_GRASS_GROWTH_FACTOR) spread = TALL_GRASS;

            } else if (biome == FLAT) {
                if (type == WATER || (type == TALL_GRASS && proba() < BLEND_FLAT_SPREAD_PROBABILITY))
                    spread = FLAT;
                else if (proba() < BLEND_TALL_GRASS_GROWTH_FACTOR * 0.11) {
                    spread = TALL_GRASS;
                }

            } else {
                spread = biome;
            }

            map->tileset[ty + cy][tx + cx].type = spread;
        }
    }

}

float generateStochasticWalk(float scale, float drift, float remaining) {
    float kick = randomFloat(-ROAD_KICK_FACTOR, ROAD_KICK_FACTOR);
    float interpFactor = (ROAD_DRIFT_FACTOR + ROAD_TARGET_ATTRACTION_FACTOR / remaining) * scale;
    return (kick + drift * interpFactor) / (1 + interpFactor);
}

bool placeRoad(Map *map, int x, int y) {
    if (map->tileset[y][x].type == BOULDER || map->tileset[y][x].type == BOULDER_ROAD) {
        map->tileset[y][x].type = BOULDER_ROAD;
        return true;
    } else {
        map->tileset[y][x].type = ROAD;
        return false;
    }
}

void placeChunk(Map *map, TileType type, int x, int y, int sizeX, int sizeY) {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            map->tileset[y + j][x + i].type = type;
        }
    }
}

void placeKernelChunk(Map *map, TileType type, int x, int y, float kernelRadius) {
    int rad = (int) floorf(kernelRadius);
    float kernelRadiusSq = kernelRadius * kernelRadius;
    for (int i = -rad; i < rad; i++) {
        for (int j = -rad; j < rad; j++) {
            if ((float) (i * i + j * j) + 1.0f > kernelRadiusSq) continue;
            map->tileset[y + j][x + i].type = type;
        }
    }
}

void generateMap(Map *map, int worldSeed, bool useBadApple) {
    int frameIdx = (int) floor(map->mapSeed * 30.0 / 1000.0);
    float sliceZ = (float) (map->mapSeed & 0xffff) / 17.477f;

    int numPoints = NOISE_DENSITY * FIRST_PASS_NUM_TYPES;
    int voronoiSeed = useBadApple ?
                      VORONOI_POINTS_BASE_SEED :
                      ((map->mapSeed & 0x7fffff) ^ VORONOI_POINTS_BASE_SEED) % 10000;
    VoronoiPoint voronoiPoints[numPoints];
    initializeVoronoiPoints(
        voronoiPoints,
        NOISE_DENSITY,
        FIRST_PASS_NUM_TYPES,
        sliceZ,
        voronoiSeed
    );

    srand(map->mapSeed);

    // Populate and generate natural tile types
    TileType firstPassTileTypes[FIRST_PASS_NUM_TYPES] = {
        FLAT,   // Short grass has greater weight
        FLAT,
        TALL_GRASS,
        WATER,
//        BOULDER,
    };
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
//            for (int i = 0; i < numPoints; i++) {
            Vec3 tilePosition = {
                (float) x * NOISE_SCALE,
                (float) y * NOISE_SCALE * VERTICAL_SCALING_FACTOR,
                sliceZ
            };
            int sampledType = sampleVoronoiNoise(voronoiPoints, numPoints, tilePosition, NOISE_LACUNARITY);
            map->tileset[y][x].type = firstPassTileTypes[sampledType];
        }
    }

    if (useBadApple) {
        char filename[42];
        sprintf(filename, "assets/sequence/badapple-%05d.png", positiveMod(frameIdx, MAX_FRAME_COUNT));
        int **frame = parse_frame(filename);
        for (int y = 1; y < MAP_HEIGHT - 1; y++) {
            for (int x = 1; x < MAP_WIDTH - 1; x++) {
                if (frame[y - 1][x - 1] > 128) map->tileset[y][x].type = BOULDER;
                else if (map->tileset[y][x].type == BOULDER) map->tileset[y][x].type = TALL_GRASS;
            }
        }
        // Clean up?
        for (int i = 0; i < MAP_HEIGHT - 2; i++) free(frame[i]);
        free(frame);
    } else { // Otherwise, scatter boulders randomly
        // Number of boulders can vary
        int boulderCount = randomInt(-SCATTER_BOULDER_VARIABILITY, SCATTER_BOULDER_VARIABILITY);
        for (int _ = 0;
             _ < SCATTER_BOULDER_COUNT * MAX_ITER_MULTIPLIER && boulderCount < SCATTER_BOULDER_COUNT;
             _++) {
            int scatterX = randomInt(PADDING, MAP_WIDTH - PADDING);
            int scatterY = randomInt(PADDING, MAP_HEIGHT - PADDING);

            if (map->tileset[scatterY][scatterX].type != WATER || proba() < SCATTER_BOULDER_ON_WATER_PROBABILITY) {
                float radius = maxf(1.0f, randomFloat(-4.0f, SCATTER_BOULDER_MAX_RADIUS));
                placeKernelChunk(map, BOULDER, scatterX, scatterY, radius);
                boulderCount++;
            }
        }
    }

    // Biome blending pass
    int distortionEdgeX = MAP_WIDTH - DISTORTION_PADDING;
    int distortionEdgeY = MAP_HEIGHT - DISTORTION_PADDING;
    for (int i = 0; i < DISTORTION_ITERATIONS_BASELINE; i++) {
        int tx = randomInt(DISTORTION_PADDING, distortionEdgeX);
        int ty = randomInt(DISTORTION_PADDING, distortionEdgeY);
        TileType biome = map->tileset[ty][tx].type;
        float p = 0.0f;
        while (p < DISTORTION_EXP_PROBABILITY
               && (tx > DISTORTION_PADDING) && (tx < distortionEdgeX)
               && (ty > DISTORTION_PADDING) && (ty < distortionEdgeY)) {
            applyBiomeBlending(map, tx, ty, biome);

            tx += (int) floorf(randomFloat(-DISTORTION_ENERGY, DISTORTION_ENERGY));
            p = proba();
        }
    }
    // 0 1 2 3 4

    // Define gate positions

    int latticeX = 2 * map->globalX;
    int latticeY = 2 * map->globalY;
    int westGateY = globalHashFunction(latticeX - 1, latticeY, worldSeed);
    int eastGateY = globalHashFunction(latticeX + 1, latticeY, worldSeed);
    int northGateX = globalHashFunction(latticeX, latticeY - 1, worldSeed);
    int southGateX = globalHashFunction(latticeX, latticeY + 1, worldSeed);
    westGateY = positiveMod(westGateY, MAP_HEIGHT - 2 * GATE_PADDING) + GATE_PADDING;
    eastGateY = positiveMod(eastGateY, MAP_HEIGHT - 2 * GATE_PADDING) + GATE_PADDING;
    northGateX = positiveMod(northGateX, MAP_WIDTH - 4 * GATE_PADDING) + 2 * GATE_PADDING;
    southGateX = positiveMod(southGateX, MAP_WIDTH - 4 * GATE_PADDING) + 2 * GATE_PADDING;

    bool pokeCenterOnVertical = randomInt(0, 1);
    int horizontalPlacement = randomInt(PADDING * 2, MAP_WIDTH - PADDING * 2);
    int verticalPlacement = randomInt(PADDING * 2, MAP_HEIGHT - PADDING * 2);

    // Start placing roads
    srand(map->mapSeed);

    int rx = 1;
    int ry = westGateY;
    float splineVelocity = 0.0f;
    for (; rx < MAP_WIDTH - 1; rx++) {
        float remaining = (float) (MAP_WIDTH - 2 - rx);
        int dy;
        float drift = (float) (eastGateY - ry);
        if (remaining > 0) {
            splineVelocity = ROAD_MOMENTUM_COEFF * splineVelocity +
                             (1 - ROAD_MOMENTUM_COEFF) * generateStochasticWalk(1.5f, drift, remaining);
            dy = (int) floorf(splineVelocity + 0.5f);
        } else {
            dy = (int) drift;
        }

        if (rx == horizontalPlacement) {
            int dir = signum(dy);
            dir = (dir == 0 ? 1 : dir);
            placeRoad(map, rx, ry - dir);
            placeRoad(map, rx, ry - 2 * dir);
            placeChunk(
                map, pokeCenterOnVertical ? POKEMART : POKECENTER,
                rx, ry - 3 * dir, 2, 2
            );
        }

        if (placeRoad(map, rx, ry) && remaining > 1) continue;
        for (int i = 0; i < abs(dy); i++) {
            ry = clamp(ry + signum(dy), PADDING, MAP_HEIGHT - PADDING);
            placeRoad(map, rx, ry);
        }
    }
    rx = northGateX;
    ry = 1;
    splineVelocity = 0.0f;
    for (; ry < MAP_HEIGHT - 1; ry++) {
        float remaining = (float) (MAP_HEIGHT - 2 - ry) * 1.3f;
        int dx;
        float drift = (float) (southGateX - rx);
        if (remaining > 1) {
            splineVelocity = ROAD_MOMENTUM_COEFF * splineVelocity +
                             (1 - ROAD_MOMENTUM_COEFF) * generateStochasticWalk(2.3f, drift, remaining);
            dx = (int) floorf(splineVelocity + 0.5f) + randomInt(-1, 1);
        } else {
            dx = (int) drift;
        }

        if (ry == verticalPlacement) {
            int dir = signum(dx);
            dir = (dir == 0 ? 1 : dir);
            placeRoad(map, rx - dir, ry);
            placeRoad(map, rx - 2 * dir, ry);
            placeChunk(
                map, pokeCenterOnVertical ? POKECENTER : POKEMART,
                rx - 3 * dir, ry, 2, 2
            );
        }


        if (placeRoad(map, rx, ry) && remaining > 1) continue;
        for (int i = 0; i < abs(dx); i++) {
            rx = clamp(rx + signum(dx), PADDING, MAP_WIDTH - PADDING);
            placeRoad(map, rx, ry);
        }
    }

    // Fill borders and place gates
    for (int x = 0; x < MAP_WIDTH; x++) {
        map->tileset[0][x].type = BORDER;
        map->tileset[MAP_HEIGHT - 1][x].type = BORDER;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        map->tileset[y][0].type = BORDER;
        map->tileset[y][MAP_WIDTH - 1].type = BORDER;
    }
    map->tileset[westGateY][0].type = GATE;
    map->tileset[eastGateY][MAP_WIDTH - 1].type = GATE;
    map->tileset[0][northGateX].type = GATE;
    map->tileset[MAP_HEIGHT - 1][southGateX].type = GATE;
}
