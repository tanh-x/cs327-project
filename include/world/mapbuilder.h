#include <stdbool.h>

#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

typedef enum __attribute__ ((__packed__)) {
    FLAT,       // .    <1st pass>
    BOULDER,    // %    <1st pass>
    BORDER,     // %    <border pass>
    GATE,       // #    <border pass>
    ROAD,       // #    <2nd pass>
    BOULDER_ROAD,// =   <2nd pass>
    TALL_GRASS, // :    <1st pass>
    WATER,      // ~    <1st pass>
    TREE,       // ^    <1st pass>
    POKECENTER, // C    <2nd pass>
    POKEMART,   // M    <2nd pass>
    JOULDER,    // )    <2nd pass>
} TileType;

typedef struct {
    TileType type;
} Tile;

typedef struct {
    Tile tileset[MAP_HEIGHT][MAP_WIDTH];
    int mapSeed;
    int globalX;
    int globalY;

    float overgrowth;
} Map;

void generateMap(Map *map, int worldSeed, bool useBadApple);

#endif
