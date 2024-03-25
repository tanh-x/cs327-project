#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include "core/constants.hpp"

#define DISTANCE_FIELD_MEMOIZATION_SIZE 40

struct DistanceField;

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
    struct DistanceField* memoizedDistanceFields[DISTANCE_FIELD_MEMOIZATION_SIZE];

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

void generateMap(Map* map, MapEntryProps* entryProps, int worldSeed, bool useBadApple);

void disposeMap(Map* map);

// Checks whether the position is inside the map, including the borders.
bool isInsideMapBounds(int x, int y);

// Checks whether the position is within the map borders, i.e. excluding the borders and the gate.
bool isInsideMapBorders(int x, int y);

#endif
