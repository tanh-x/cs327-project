#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/world/mapbuilder.h"
#include "../../include/utils/mathematics.h"
#include "../../include/graphics/parse_frame.h"
#include "../../include/utils/voronoi_noise.h"

#define MAX_FRAME_COUNT 6572

#define GATE_PLACEMENT_PADDING 6
#define FIRST_PASS_NUM_TYPES 4

#define NOISE_DENSITY 16
#define NOISE_SCALE 0.925f
#define NOISE_LACUNARITY 8.3242f

#define DISTORTION_EXP_PROBABILITY 0.8453f
#define DISTORTION_ITERATIONS 32
#define DISTORTION_PADDING 3
#define DISTORTION_KERNEL_RADIUS_SQ 7.54f
#define DISTORTION_ENERGY 3.1416f

void initializeMap(Map *map, bool useBadApple) {
    int frameIdx = (int) floor(map->mapSeed * 30.0 / 1000.0);
    float sliceZ = (float) (map->mapSeed & 0xffff) / 17.477f;

    int numPoints = NOISE_DENSITY * FIRST_PASS_NUM_TYPES;
    VoronoiPoint voronoiPoints[numPoints];
    initializeVoronoiPoints(voronoiPoints, NOISE_DENSITY, FIRST_PASS_NUM_TYPES, sliceZ, frameIdx);

    srand(map->mapSeed);

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
                else if (map->tileset[y][x].type == BOULDER) map->tileset[y][x].type = TREE;
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
        int biome = map->tileset[ty][tx].type;
        float p = 1.0f;
        while (p > DISTORTION_EXP_PROBABILITY
               && (tx > DISTORTION_PADDING) && (tx < distortionEdgeX)
               && (ty > DISTORTION_PADDING) && (ty < distortionEdgeY)) {
            for (int cx = -2; cx <= 2; cx++) {
                for (int cy = -2; cy <= 2; cy++) {
                    if ((float) (cx * cx + cy * cy) > DISTORTION_KERNEL_RADIUS_SQ) continue;
                    map->tileset[ty + cy][tx + cx].type = biome;
                }
            }

            tx += (int) floorf(randomFloat(-DISTORTION_ENERGY, DISTORTION_ENERGY));
            p = randomFloat(0.0f, 1.0f);
        }
    }

    // Define gate positions
    int westGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int eastGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int northGateX = randomInt(GATE_PLACEMENT_PADDING, MAP_WIDTH - GATE_PLACEMENT_PADDING);
    int southGateX = randomInt(GATE_PLACEMENT_PADDING, MAP_WIDTH - GATE_PLACEMENT_PADDING);

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

    // Place trees

    // Finalize
}
