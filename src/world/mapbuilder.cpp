#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "core/game_manager.hpp"
#include "entities/pathfinding.hpp"
#include "graphics/parse_frame.hpp"
#include "utils/mathematics.hpp"
#include "utils/voronoi_noise.hpp"
#include "world/mapbuilder.hpp"

#define MAX_FRAME_COUNT 6572
#define MAX_ITER_MULTIPLIER 8
#define OVERGROWTH_FACTOR 12.6277f

#define FIRST_PASS_NUM_TYPES 4

#define BUILDING_FALLOFF_FACTOR 63.85f
#define BUILDING_FALLOFF_INTERCEPT 70.85f

#define NOISE_DENSITY 17
#define NOISE_SCALE 1.035f
#define NOISE_LACUNARITY 10.1042f

#define SCATTER_BOULDER_COUNT 24
#define SCATTER_BOULDER_VARIABILITY 5
#define SCATTER_BOULDER_ON_WATER_PROBABILITY 0.1277f
#define SCATTER_BOULDER_MAX_RADIUS 2.13f
#define PURPLE_BOULDER_PROBABILITY 0.0159f

#define DISTORTION_EXP_PROBABILITY 0.7563f
#define DISTORTION_ITERATIONS_BASELINE 100
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
#define BLEND_FLAT_SPREAD_PROBABILITY 0.5522f

#define ROAD_MOMENTUM_COEFF 0.7621f
#define ROAD_DRIFT_FACTOR 0.121f
#define ROAD_KICK_FACTOR 3.12f
#define ROAD_TARGET_ATTRACTION_FACTOR 2.2572f

#define EDGE_STUB_PADDING_X 13
#define EDGE_STUB_PADDING_Y 6
#define EDGE_WEST_BITMASK 1u
#define EDGE_EAST_BITMASK 2u
#define EDGE_SOUTH_BITMASK 4u
#define EDGE_NORTH_BITMASK 8u

#define VORONOI_POINTS_BASE_SEED 21679733

#define WILDERNESS_LEVEL_LOWER_BOUND 0.0425f
#define WILDERNESS_LEVEL_UPPER_BOUND 0.1625f

#define MENACE_INITIAL_PERTURBATION_SCALAR_FACTOR 0.1375f
#define MENACE_SALIENT_MAP_BONUS 38.71f

std::string locationNamePrefixes[] = LOCATION_NAME_PREFIXES;
std::string locationNameSuffixes[] = LOCATION_NAME_SUFFIXES;
int prefixesSize = sizeof(locationNamePrefixes) / sizeof(locationNamePrefixes[0]);
int suffixesSize = sizeof(locationNameSuffixes) / sizeof(locationNameSuffixes[0]);

// Initializes the map, but does not yet generate the terrain, which is deferred until the method
// generateTerrain(int worldSeed) is invoked. It also does not initialize an entity manager, which
// is deferred until setupGameOnMapLoad is called for the first time on this map.
Map::Map(int globalX, int globalY, int initialSeed) {
    auto dist = float(abs(globalX) + abs(globalY));
    float eccentricity = GAME.world->eccentricity[globalY + WORLD_Y_SPAN][globalX + WORLD_X_SPAN];

    this->globalX = globalX;
    this->globalY = globalY;
    this->isSpawnMap = globalX == 0 && globalY == 0;
    this->mapSeed = globalHashFunction(globalX, globalY, initialSeed);
    this->overgrowth = OVERGROWTH_FACTOR * sqrtf(dist) * randomFloat(0.55f, 3.25f);
    this->menaceLevel = getMenaceLevel(globalX, globalY, eccentricity, GAME.world->worldSeed);
this->wildernessLevel = randomFloat(WILDERNESS_LEVEL_LOWER_BOUND, WILDERNESS_LEVEL_UPPER_BOUND)
                            + overgrowth / 2100.0f;

    this->numOpponents = 0;
    this->entityManager = nullptr;

    srand(mapSeed);
    // Get a random prefix and suffix
    int prefixIndex = rand() % prefixesSize;
    int suffixIndex = rand() % suffixesSize;

    // Concatenate the random name
    this->name = locationNamePrefixes[prefixIndex] + " " + locationNameSuffixes[suffixIndex];
}

Map::~Map() {
    invalidateMemoization(memoizedDistanceFields);
    this->entityManager = nullptr;
}

float calculateBuildingProbability(int globalX, int globalY) {
    return (-BUILDING_FALLOFF_FACTOR * (float) (abs(globalX) + abs(globalY)) / 200.0f + BUILDING_FALLOFF_INTERCEPT) /
           100.0f;
}

float generateStochasticWalk(float scale, float drift, float remaining) {
    float kick = randomFloat(-ROAD_KICK_FACTOR, ROAD_KICK_FACTOR);
    float interpFactor = (ROAD_DRIFT_FACTOR + ROAD_TARGET_ATTRACTION_FACTOR / remaining) * scale;
    return (kick + drift * interpFactor) / (1 + interpFactor);
}

// @formatter:off
bool Map::placeRoad(int x, int y, uint8_t edgeBitmask) {
    bool isNorthEdge = edgeBitmask & EDGE_NORTH_BITMASK;
    bool isSouthEdge = edgeBitmask & EDGE_SOUTH_BITMASK;
    bool isEastEdge = edgeBitmask & EDGE_EAST_BITMASK;
    bool isWestEdge = edgeBitmask & EDGE_WEST_BITMASK;

    if ((isWestEdge && x < EDGE_STUB_PADDING_X)
        || (isEastEdge && x >= MAP_WIDTH - EDGE_STUB_PADDING_X)
    ) {
        x = clamp(x, EDGE_STUB_PADDING_X, MAP_WIDTH - EDGE_STUB_PADDING_X);
    }

    if ((isNorthEdge && y < EDGE_STUB_PADDING_Y)
        || (isSouthEdge && y >= MAP_HEIGHT - EDGE_STUB_PADDING_Y)
    ) {
        y = clamp(y, EDGE_STUB_PADDING_Y, MAP_HEIGHT - EDGE_STUB_PADDING_Y);
    }

    if (tileset[y][x].type == BOULDER || tileset[y][x].type == BOULDER_ROAD) {
        tileset[y][x].type = BOULDER_ROAD;
        return true;
    }

    tileset[y][x].type = ROAD;
    return false;
}
// @formatter:on

void Map::placeChunk(TileType type, int x, int y, int sizeX, int sizeY) {
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeY; j++) {
            tileset[y + j][x + i].type = type;
        }
    }
}

void Map::placeKernelChunk(TileType type, int x, int y, float kernelRadius) {
    int rad = (int) floorf(kernelRadius);
    float kernelRadiusSq = kernelRadius * kernelRadius;
    for (int i = -rad; i < rad; i++) {
        for (int j = -rad; j < rad; j++) {
            if ((float) (i * i + j * j) + 1.0f > kernelRadiusSq) continue;
            tileset[y + j][x + i].type = type;
        }
    }
}

MapEntryProps Map::generateTerrain(int worldSeed) {
    MapEntryProps entryProps;
    bool useBadApple = OPTIONS.doBadApple;

    uint8_t edgeBitmask = 0;
    if (globalY == -WORLD_Y_SPAN) edgeBitmask |= EDGE_NORTH_BITMASK;
    if (globalY == +WORLD_Y_SPAN) edgeBitmask |= EDGE_SOUTH_BITMASK;
    if (globalX == +WORLD_X_SPAN) edgeBitmask |= EDGE_EAST_BITMASK;
    if (globalX == -WORLD_X_SPAN) edgeBitmask |= EDGE_WEST_BITMASK;

    int frameIdx = (int) floor(mapSeed * 30.0 / 1000.0);
    float sliceZ = (float) (mapSeed & 0xffff) / 17.477f;

    //  1. First pass: Voronoi noise
    //      NOISE_DENSITY points of each of the 4 types (short/tall grass, boulder, water)
    //      scattered uniformly in 3D space. Initial terrain types are then generated
    //      by sampling the corresponding voronoi diagram, i.e., the closest point will
    //      determine the type of terrain at that cell. The sampling point has a small
    //      random factor to make the terrain more natural.
    //
    //      The 3rd dimension allows for smooth tweaks of the terrain by sliding along
    //      the z-axis, as opposed to using a new seed, which will always generate a
    //      completely different terrain map.
    //
    //      Since the y-axis covers more space than the x-axis (text is tall, rather
    //      than wide), the coordinate space is also scaled accordingly to minimize
    //      distortion.
    //
    //      This first pass will define overall shape of the terrain, but the appearance
    //      will be similar to that of a voronoi diagram, which is too geometric. We
    //      need further processing to make the terrain more natural

    int numPoints = NOISE_DENSITY * FIRST_PASS_NUM_TYPES;
    int voronoiSeed = useBadApple ?
                      VORONOI_POINTS_BASE_SEED :
                      ((mapSeed & 0x7fffff) ^ VORONOI_POINTS_BASE_SEED) % 10000;
    VoronoiPoint voronoiPoints[numPoints];
    initializeVoronoiPoints(
        voronoiPoints,
        NOISE_DENSITY,
        FIRST_PASS_NUM_TYPES,
        sliceZ,
        voronoiSeed
    );

    srand(mapSeed);

    // Populate and generate natural tile types
    TileType firstPassTileTypes[FIRST_PASS_NUM_TYPES] = {
        FLAT,   // Short grass has greater weight
        FLAT,
        TALL_GRASS,
        WATER,
    };
    for (int y = 1; y < MAP_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_WIDTH - 1; x++) {
            Float3D tilePosition = {
                (float) x * NOISE_SCALE,
                (float) y * NOISE_SCALE * VERTICAL_SCALING_FACTOR,
                sliceZ
            };
            int sampledType = sampleVoronoiNoise(voronoiPoints, numPoints, tilePosition, NOISE_LACUNARITY);
            tileset[y][x].type = firstPassTileTypes[sampledType];
        }
    }

    if (useBadApple) {
        char filename[42];
        sprintf(filename, "assets/sequence/badapple-%05d.png", positiveMod(frameIdx, MAX_FRAME_COUNT));
        int** frame = parse_frame(filename);
        for (int y = 1; y < MAP_HEIGHT - 1; y++) {
            for (int x = 1; x < MAP_WIDTH - 1; x++) {
                if (frame[y - 1][x - 1] > 128) tileset[y][x].type = BOULDER;
                else if (tileset[y][x].type == BOULDER) tileset[y][x].type = TALL_GRASS;
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

            if (tileset[scatterY][scatterX].type != WATER || proba() < SCATTER_BOULDER_ON_WATER_PROBABILITY) {
                if (proba() < PURPLE_BOULDER_PROBABILITY) {
                    placeChunk(JOULDER, scatterX, scatterY, 1, 1);
                } else {
                    float radius = maxf(1.0f, randomFloat(-4.0f, SCATTER_BOULDER_MAX_RADIUS));
                    placeKernelChunk(BOULDER, scatterX, scatterY, radius);
                }
                boulderCount++;
            }
        }
    }

    if (edgeBitmask & EDGE_WEST_BITMASK) {
        placeChunk(WATER, 1, 1, EDGE_STUB_PADDING_X, MAP_HEIGHT - 2);
    } else if (edgeBitmask & EDGE_EAST_BITMASK) {
        placeChunk(WATER, MAP_WIDTH - EDGE_STUB_PADDING_X, 1, EDGE_STUB_PADDING_X, MAP_HEIGHT - 2);
    }

    if (edgeBitmask & EDGE_NORTH_BITMASK) {
        placeChunk(WATER, 1, 1, MAP_WIDTH - 2, EDGE_STUB_PADDING_Y + 3);
    } else if (edgeBitmask & EDGE_SOUTH_BITMASK) {
        placeChunk(WATER, 1, MAP_HEIGHT - EDGE_STUB_PADDING_Y - 4, MAP_WIDTH - 2, EDGE_STUB_PADDING_Y + 3);
    }

    // 2. Second pass: "Biome blending" / quasi cellular automaton
    //    Choose one tile at random ("pivot point"), and consider the neighborhood of
    //    12 adjacent points whose taxicab distance is less than 2 from the pivot.
    //    These points have a chance to be converted to a different type, whose rules
    //    depend on the type of the pivot and the type of the current point. This can
    //    be thought of as a cellular automaton system. For more details, see the
    //    biome blending rules section in the README.
    //
    //    Once we're done, with DISTORTION_EXP_PROBABILITY, continue the process by
    //    moving the pivot along a random direction by a random amount (0-2 tiles).
    //    However, we keep the pivot type of the original one. This allows some chosen
    //    tiles to have a far-reaching influence. Repeat until we hit the stop condition.
    //
    //    The number of iterations is determined by the overgrowth factor of the which
    //    allows for the number of iteratons to range from 50-300.

    int distortionEdgeX = MAP_WIDTH - DISTORTION_PADDING;
    int distortionEdgeY = MAP_HEIGHT - DISTORTION_PADDING;
    int iterations = DISTORTION_ITERATIONS_BASELINE + clamp((int) floorf(overgrowth), 0, 260);
    for (int i = 0; i < iterations; i++) {
        int tx = randomInt(DISTORTION_PADDING, distortionEdgeX);
        int ty = randomInt(DISTORTION_PADDING, distortionEdgeY);
        TileType biome = tileset[ty][tx].type;
        float p = 0.0f;
        while (p < DISTORTION_EXP_PROBABILITY
               && (tx > DISTORTION_PADDING) && (tx < distortionEdgeX)
               && (ty > DISTORTION_PADDING) && (ty < distortionEdgeY)) {
            applyBiomeBlending(tx, ty, biome);

            tx += (int) floorf(randomFloat(-DISTORTION_ENERGY, DISTORTION_ENERGY));
            p = proba();
        }
    }

    // 3. Third pass: Road and buildings
    //    Road traversal start from the left and top. For the horizontal road, every
    //    step, the road traverses to the right by 1 tile, and goes up/down by `dy`
    //    tiles. `dy` is initially 0, but for every step, a random "kick" is applied
    //    to `dy`. We don't want our paths to be jagged, so `dy` has a "momentum"
    //    factor. Finally, `dy` is also biased by a "drift" factor that pulls the path
    //    towards the gate on the other side. The closer we are to the eastern border,
    //    the stronger the drift factor. A similar process is done for the vertical.
    //
    //    For buildings, a random point on a road is chosen prior to road generation.
    //    During generation, we know that the road will bend towards `dy` (or `dy`),
    //    so we place buildings 1-3 tiles away on the other direction, `-dy`, then we
    //    connect the building to the main road with a simple straight path.

    // Define gate positions
    int westGateY = hashWithMapCardinalDir(globalX, globalY, WEST, worldSeed);
    int eastGateY = hashWithMapCardinalDir(globalX, globalY, EAST, worldSeed);
    int northGateX = hashWithMapCardinalDir(globalX, globalY, NORTH, worldSeed);
    int southGateX = hashWithMapCardinalDir(globalX, globalY, SOUTH, worldSeed);

    bool pokeCenterOnVertical = randomInt(0, 1);
    float buildingProbability = calculateBuildingProbability(globalX, globalY);
    int horizontalPlacementX = (proba() < buildingProbability || isSpawnMap)
                               ? randomInt(PADDING * 2, MAP_WIDTH - PADDING * 2)
                               : -1;
    int verticalPlacementY = (proba() < buildingProbability || isSpawnMap)
                             ? randomInt(PADDING * 2, MAP_HEIGHT - PADDING * 2)
                             : -1;

    // Start placing roads
    srand(mapSeed);

    int rx = 1;
    int ry = westGateY;
    float splineVelocity = 0.0f;
    for (; rx < MAP_WIDTH - 1; rx++) {
        auto remaining = (float) (MAP_WIDTH - 2 - rx);
        int dy;
        auto drift = (float) (eastGateY - ry);
        if (remaining > 0) {
            splineVelocity = ROAD_MOMENTUM_COEFF * splineVelocity +
                             (1 - ROAD_MOMENTUM_COEFF) * generateStochasticWalk(1.5f, drift, remaining);
            dy = (int) floorf(splineVelocity + 0.5f);
        } else {
            dy = (int) drift;
        }

        if (rx == horizontalPlacementX) {
            int dir = signum(dy);
            dir = (dir == 0 ? 1 : dir);
            placeRoad(rx, ry - dir, edgeBitmask);
            placeRoad(rx, ry - 2 * dir, edgeBitmask);
            placeChunk(
                pokeCenterOnVertical ? POKEMART : POKECENTER,
                rx, ry - 3 * dir, 2, 2
            );

            // Make player spawn here, if it's the spawnNpc map
            if (isSpawnMap) {
                entryProps.playerSpawnX = horizontalPlacementX;
                entryProps.playerSpawnY = ry;
            }
        }

        if (placeRoad(rx, ry, edgeBitmask) && remaining > 1) continue;
        for (int i = 0; i < abs(dy); i++) {
            ry = clamp(ry + signum(dy), PADDING, MAP_HEIGHT - PADDING);
            placeRoad(rx, ry, edgeBitmask);
        }
    }
    rx = northGateX;
    ry = 1;
    splineVelocity = 0.0f;
    for (; ry < MAP_HEIGHT - 1; ry++) {
        float remaining = (float) (MAP_HEIGHT - 2 - ry) * 1.3f;
        int dx;
        auto drift = (float) (southGateX - rx);
        if (remaining > 1) {
            splineVelocity = ROAD_MOMENTUM_COEFF * splineVelocity +
                             (1 - ROAD_MOMENTUM_COEFF) * generateStochasticWalk(2.3f, drift, remaining);
            dx = (int) floorf(splineVelocity + 0.5f) + randomInt(-1, 1);
        } else {
            dx = (int) drift;
        }

        if (ry == verticalPlacementY) {
            int dir = signum(dx);
            dir = (dir == 0 ? 1 : dir);
            placeRoad(rx - dir, ry, edgeBitmask);
            placeRoad(rx - 2 * dir, ry, edgeBitmask);
            placeChunk(
                pokeCenterOnVertical ? POKECENTER : POKEMART,
                rx - 3 * dir, ry, 2, 2
            );
        }


        if (placeRoad(rx, ry, edgeBitmask) && remaining > 1) continue;
        for (int i = 0; i < abs(dx); i++) {
            rx = clamp(rx + signum(dx), PADDING, MAP_WIDTH - PADDING);
            placeRoad(rx, ry, edgeBitmask);
        }
    }

    // Fill borders and place gates
    // NOLINTBEGIN
    for (int x = 0; x < MAP_WIDTH; x++) {
        tileset[0][x].type = BORDER;
        tileset[MAP_HEIGHT - 1][x].type = BORDER;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        tileset[y][0].type = BORDER;
        tileset[y][MAP_WIDTH - 1].type = BORDER;
    }
    // NOLINTEND

    if (!(edgeBitmask & EDGE_WEST_BITMASK)) tileset[westGateY][0].type = GATE;
    if (!(edgeBitmask & EDGE_EAST_BITMASK)) tileset[eastGateY][MAP_WIDTH - 1].type = GATE;
    if (!(edgeBitmask & EDGE_NORTH_BITMASK)) tileset[0][northGateX].type = GATE;
    if (!(edgeBitmask & EDGE_SOUTH_BITMASK)) tileset[MAP_HEIGHT - 1][southGateX].type = GATE;

    return entryProps;
}

void Map::applyBiomeBlending(int tx, int ty, TileType biome) {
    for (int cx = -2; cx <= 2; cx++) {
        for (int cy = -1; cy <= 1; cy++) {
            if ((float) (cx * cx + cy * cy) > DISTORTION_KERNEL_RADIUS_SQ) continue;
            TileType type = tileset[ty + cy][tx + cx].type;

            if (type == BOULDER && proba() < BLEND_BOULDER_EROSION_PROBABILITY) continue;

            TileType spread = type;
            if (biome == BOULDER) {
                // Boulders may sprout nearby tall grass or water tiles into trees, with a probability of
                // BLEND_SPROUTING_PROBABILITY. Otherwise, it has a BLEND_BOULDER_SPREAD_PROBABILITY probability
                // to convert any tile to another boulder.
                if ((type == TALL_GRASS || type == WATER)
                    && proba() < BLEND_SPROUTING_PROBABILITY)
                    spread = TREE;
                else if (proba() < BLEND_BOULDER_SPREAD_PROBABILITY) spread = BOULDER;

            } else if (biome == WATER) {
                // Each nearby boulder tile has a BLEND_BOULDER_WATER_EROSION_PROBABILITY chance to become water.
                if (type == BOULDER && proba() < BLEND_BOULDER_WATER_EROSION_PROBABILITY) spread = WATER;

            } else if (biome == TREE) {
                // A tree can spread to any neighboring tiles, with BLEND_TREE_SPREAD_PROBABILITY,
                // or a bonus factor of BLEND_TREE_SPREAD_ENVIRONMENTAL_BONUS if it's a short/tall grass tile.
                // If this succeeds, the tile will be converted to a tree. Otherwise, it is converted to tall grass.
                // So, while trees are rare, they can spread plants very well.
                float p = proba();
                if (type == TALL_GRASS || type == FLAT) p /= BLEND_TREE_SPREAD_ENVIRONMENTAL_BONUS;
                if (p < BLEND_TREE_SPREAD_PROBABILITY) spread = TREE;
                else spread = TALL_GRASS;

            } else if (biome == TALL_GRASS) {
                // A tall grass have a BLEND_SPROUTING_PROBABILITY of converting neighboring tall grass or
                // water tiles to sprout a new tree. If this fails, it is guaranteed to convert neighboring water
                // or boulders tiles to tall grass. Neighboring short grass tiles might also be grown to tall grass,
                // with BLEND_TALL_GRASS_GROWTH_FACTOR probability.
                if ((type == TALL_GRASS || type == WATER) &&
                    proba() < BLEND_SPROUTING_PROBABILITY * 0.4)
                    spread = TREE;
                    // NOLINTBEGIN
                else if (type == WATER || type == BOULDER) {
                    spread = TALL_GRASS;
                    // NOLINTEND
                } else if (type == FLAT && proba() < BLEND_TALL_GRASS_GROWTH_FACTOR) {
                    spread = TALL_GRASS;
                }

            } else if (biome == FLAT) {
                // Short grass has a BLEND_FLAT_SPREAD_PROBABILITY to grow into nearby tall grass or water tiles.
                if (type == WATER || (type == TALL_GRASS && proba() < BLEND_FLAT_SPREAD_PROBABILITY))
                    spread = FLAT;
                else if (proba() < BLEND_TALL_GRASS_GROWTH_FACTOR * 0.11) {
                    spread = TALL_GRASS;
                }

            } else {
                continue;
            }

            tileset[ty + cy][tx + cx].type = spread;
        }
    }

}

// Checks whether the position is inside the including the borders.
bool isInsideMapBounds(int x, int y) {
    return x >= 0 && y >= 0 && x <= MAP_WIDTH - 1 && y <= MAP_HEIGHT - 1;
}

// Checks whether the position is within the map borders, i.e. excluding the borders and the gate.
bool isInsideMapBorders(int x, int y) {
    return x > 0 && y > 0 && x < MAP_WIDTH - 1 && y < MAP_HEIGHT - 1;
}

float getMenaceLevel(int globalX, int globalY, float eccentricity, int worldSeed) {
    float perturbation = eccentricity + float(globalHashFunction(globalX, globalY, worldSeed) % 36);
    perturbation *= MENACE_INITIAL_PERTURBATION_SCALAR_FACTOR;
    if (eccentricity > SALIENCY_THRESHOLD) perturbation += MENACE_SALIENT_MAP_BONUS;
    return float(abs(globalX) + abs(globalY)) + perturbation;
}

