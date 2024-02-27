#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <stdbool.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21

#define PADDING 3
#define GATE_PADDING 7

typedef enum __attribute__ ((__packed__)) {
    FLAT,       // .    <1st pass>
    BOULDER,    // %    <1st pass>
    BORDER,     // %    <border pass>
    MOUNTAIN,   // %
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
    bool isSpawnMap;

    float overgrowth;
} Map;


typedef struct {
    int playerSpawnX;
    int playerSpawnY;
} MapEntryProps;

void generateMap(Map *map, MapEntryProps *entryProps, int worldSeed, bool useBadApple);

#endif
