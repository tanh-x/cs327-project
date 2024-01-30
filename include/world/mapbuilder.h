#include <stdbool.h>

#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef enum {
    FLAT,      // .
    BOULDER,    // %
    BORDER,     // %
    GATE,       // #
    ROAD,       // #
    TALL_GRASS, // :
    WATER,      // ~
    TREE,       // ^
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
