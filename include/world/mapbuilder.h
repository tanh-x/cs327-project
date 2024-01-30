#include <stdbool.h>

#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#define MAX_FRAME_COUNT 6572

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

#define GATE_PLACEMENT_PADDING 6
#define FIRST_PASS_NUM_TYPES 4

#define NOISE_SCALE 1.0f
#define NOISE_DENSITY 16
//#define NOISE_LACUNARITY 1.0006f

typedef enum {
    FLAT,       // .    <1st pass>
    BOULDER,    // %    <1st pass>
    BORDER,     // %    <border pass>
    GATE,       // #    <border pass>
    ROAD,       // #    <2nd pass>
    TALL_GRASS, // :    <1st pass>
    WATER,      // ~    <1st pass>
    TREE,       // ^    <3nd pass>
} TileType;

typedef struct {
    TileType type;
} Tile;

typedef struct {
    Tile tiles[MAP_HEIGHT][MAP_WIDTH];
    int mapSeed;
    int globalX;
    int globalY;
} Map;

void initializeMap(Map *map, bool useBadApple);

#endif
