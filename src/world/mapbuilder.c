#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/world/mapbuilder.h"
#include "../../include/utils/mathematics.h"
#include "../../include/graphics/parse_frame.h"
#include "../../include/utils/voronoi_noise.h"

#define MAX_FRAME_COUNT 6572

#define GATE_PLACEMENT_PADDING 7
#define ROAD_PLACEMENT_PADDING 2
#define FIRST_PASS_NUM_TYPES 4

#define NOISE_DENSITY 20
#define NOISE_SCALE 1.025f
#define NOISE_LACUNARITY 7.3242f

#define DISTORTION_EXP_PROBABILITY 0.5163f
#define DISTORTION_ITERATIONS 80
#define DISTORTION_PADDING 2
#define DISTORTION_KERNEL_RADIUS_SQ 4.85f
#define DISTORTION_ENERGY 2.6416f

#define BLEND_BOULDER_SPREAD_PROBABILITY 0.0452f
#define BLEND_BOULDER_EROSION_PROBABILITY 0.7211f
#define BLEND_BOULDER_WATER_EROSION_PROBABILITY 0.9511f
#define BLEND_SPROUTING_PROBABILITY 0.5222f
#define BLEND_TALL_GRASS_GROWTH_FACTOR 0.8621f
#define BLEND_TREE_SPREAD_PROBABILITY 0.1627f
#define BLEND_TREE_SPREAD_ENVIRONMENTAL_BONUS 1.2621f
#define BLEND_FLAT_SPREAD_PROBABILITY 0.6f

#define ROAD_MOMENTUM_COEFF 0.8221f
#define ROAD_DRIFT_FACTOR 0.121f
#define ROAD_KICK_FACTOR 3.52f
#define ROAD_TARGET_ATTRACTION_FACTOR 2.2572f

#define VORONOI_POINTS_SEED 21679733

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

void initializeMap(Map *map, bool useBadApple) {
    int frameIdx = (int) floor(map->mapSeed * 30.0 / 1000.0);
    float sliceZ = (float) (map->mapSeed & 0xffff) / 17.477f;
    int gateSeed = 31209474;

    int numPoints = NOISE_DENSITY * FIRST_PASS_NUM_TYPES;
    int voronoiSeed = useBadApple ?
                      VORONOI_POINTS_SEED :
                      ((map->mapSeed & 0x7fffff) ^ VORONOI_POINTS_SEED) % 10000;
    VoronoiPoint voronoiPoints[numPoints];
    initializeVoronoiPoints(
        voronoiPoints,
        NOISE_DENSITY,
        FIRST_PASS_NUM_TYPES,
        sliceZ,
        voronoiSeed
    );

    srand(map->mapSeed);

    printf("World seed: %d. Voronoi seed: %d. Gates seed: %d\n", map->mapSeed, voronoiSeed, gateSeed);
    // Populate and generate natural tile types
    TileType firstPassTileTypes[FIRST_PASS_NUM_TYPES] = {
        FLAT, BOULDER, TALL_GRASS, WATER
    };
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
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
        sprintf(filename, "assets/sequence/badapple-%05d.png", frameIdx % MAX_FRAME_COUNT);
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
    }

    // Biome blending pass
    int distortionEdgeX = MAP_WIDTH - DISTORTION_PADDING;
    int distortionEdgeY = MAP_HEIGHT - DISTORTION_PADDING;
    for (int i = 0; i < DISTORTION_ITERATIONS; i++) {
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

    // Define gate positions
    srand(gateSeed);
    int westGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int eastGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int northGateX = randomInt(GATE_PLACEMENT_PADDING * 3, MAP_WIDTH - GATE_PLACEMENT_PADDING * 3);
    int southGateX = randomInt(GATE_PLACEMENT_PADDING * 3, MAP_WIDTH - GATE_PLACEMENT_PADDING * 3);

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
            dy = (int) floorf(splineVelocity + 0.5f) + randomInt(0, 2);
        } else {
            dy = (int) drift;
        }

        if (placeRoad(map, rx, ry) && remaining > 1) continue;
        for (int i = 0; i < abs(dy); i++) {
            ry = clamp(ry + signum(dy), ROAD_PLACEMENT_PADDING, MAP_HEIGHT - ROAD_PLACEMENT_PADDING);
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
            dx = (int) floorf(splineVelocity + 0.5f) + randomInt(-1, 2);
        } else {
            dx = (int) drift;
        }
        if (placeRoad(map, rx, ry) && remaining > 1) continue;
        for (int i = 0; i < abs(dx); i++) {
            rx = clamp(rx + signum(dx), ROAD_PLACEMENT_PADDING, MAP_WIDTH - ROAD_PLACEMENT_PADDING);
            placeRoad(map, rx, ry);
        }
    }
    // Finalize
}
