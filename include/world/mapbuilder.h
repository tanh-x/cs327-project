#include <stdbool.h>

#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

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
    Tile tileset[MAP_HEIGHT][MAP_WIDTH];
    int mapSeed;
    int globalX;
    int globalY;
} Map;

void initializeMap(Map *map, bool useBadApple);

#endif
