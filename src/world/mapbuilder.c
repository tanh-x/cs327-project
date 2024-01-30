#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/world/mapbuilder.h"
#include "../../include/utils/mathematics.h"
#include "../../include/graphics/parse_frame.h"
#include "../../include/utils/voronoi_noise.h"

void initializeMap(Map *map, bool useBadApple) {
    float sliceZ = (float) (map->mapSeed & 0xffff) / 1225.477f;

    int numPoints = NOISE_DENSITY * FIRST_PASS_NUM_TYPES;
    VoronoiPoint voronoiPoints[numPoints];
    initializeVoronoiPoints(voronoiPoints, NOISE_DENSITY, FIRST_PASS_NUM_TYPES, sliceZ);

    srand(map->mapSeed);
    for (int i = 0; i < numPoints; i++) {
        printf("%d - ", voronoiPoints[i].type);
        printVector(voronoiPoints[i].position);
    }
    // Populate and generate natural tile types
    TileType firstPassTileTypes[FIRST_PASS_NUM_TYPES] = {
        FLAT, BOULDER, TALL_GRASS, WATER
    };
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            Vec3 tilePosition = {(float) x * NOISE_SCALE, (float) y * NOISE_SCALE, sliceZ};
            int sampledType = sampleVoronoiNoise(tilePosition, voronoiPoints, numPoints);
            map->tiles[y][x].type = firstPassTileTypes[sampledType];
        }
    }

    if (!useBadApple) {
        char filename[42];
        int frameIdx = (int) floor(map->mapSeed * 30.0 / 1000.0);
        sprintf(filename, "assets/sequence/badapple-%05d.png", frameIdx % MAX_FRAME_COUNT);
        int **frame = parse_frame(filename);
        for (int y = 1; y < MAP_HEIGHT - 1; y++) {
            for (int x = 1; x < MAP_WIDTH - 1; x++) {
                if (frame[y - 1][x - 1] > 128) map->tiles[y][x].type = BOULDER;
                else if (map->tiles[y][x].type == BOULDER) map->tiles[y][x].type = TREE;
            }
        }

        // Clean up?
        for (int i = 0; i < MAP_HEIGHT - 2; i++) free(frame[i]);
        free(frame);
    }

    // Define gate positions
    int westGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int eastGateY = randomInt(GATE_PLACEMENT_PADDING, MAP_HEIGHT - GATE_PLACEMENT_PADDING);
    int northGateX = randomInt(GATE_PLACEMENT_PADDING, MAP_WIDTH - GATE_PLACEMENT_PADDING);
    int southGateX = randomInt(GATE_PLACEMENT_PADDING, MAP_WIDTH - GATE_PLACEMENT_PADDING);

    // Fill borders and place gates
    for (int x = 0; x < MAP_WIDTH; x++) {
        map->tiles[0][x].type = BORDER;
        map->tiles[MAP_HEIGHT - 1][x].type = BORDER;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        map->tiles[y][0].type = BORDER;
        map->tiles[y][MAP_WIDTH - 1].type = BORDER;
    }
    map->tiles[westGateY][0].type = GATE;
    map->tiles[eastGateY][MAP_WIDTH - 1].type = GATE;
    map->tiles[0][northGateX].type = GATE;
    map->tiles[MAP_HEIGHT - 1][southGateX].type = GATE;

    // Start placing roads

    // Place trees

    // Finalize
}
