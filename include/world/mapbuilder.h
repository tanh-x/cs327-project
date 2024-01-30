#ifndef WORLDGEN_H
#define WORLDGEN_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef enum {
    EMPTY,      // .
    BOULDER,    // %
    BORDER,     // %
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

void initializeMap(Map *map);

#endif
