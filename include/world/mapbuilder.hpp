#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include "core/constants.hpp"
#include "entities/entity_manager.hpp"

#define DISTANCE_FIELD_MEMOIZATION_SIZE 40

class EntityManager;

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
    int playerSpawnX;
    int playerSpawnY;
} MapEntryProps;

class Map {
public:
    // Initializes the map, but does not yet generate the terrain, which is deferred until the method
    // generateTerrain(int worldSeed) is invoked. It also does not initialize an entity manager, which
    // is deferred until setupGameOnMapLoad is called for the first time on this map.
    Map(int globalX, int globalY, int initialSeed);

    ~Map();

    int mapSeed;
    int globalX;
    int globalY;
    bool isSpawnMap;
    float overgrowth;

    Tile tileset[MAP_HEIGHT][MAP_WIDTH] {};
    DistanceField* memoizedDistanceFields[DISTANCE_FIELD_MEMOIZATION_SIZE] {};
    EntityManager* entityManager;

    MapEntryProps generateTerrain(int worldSeed);

private:
    bool placeRoad(int x, int y, int edgeBitmask);

    void placeChunk(TileType type, int x, int y, int sizeX, int sizeY);

    void placeKernelChunk(TileType type, int x, int y, float kernelRadius);

    void applyBiomeBlending(int tx, int ty, TileType biome);
};


// Checks whether the position is inside the map, including the borders.
bool isInsideMapBounds(int x, int y);

// Checks whether the position is within the map borders, i.e. excluding the borders and the gate.
bool isInsideMapBorders(int x, int y);

#endif
