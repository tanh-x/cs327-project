#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <string>
#include "core/constants/primary.hpp"
#include "entities/entity_manager.hpp"

#define DISTANCE_FIELD_MEMOIZATION_SIZE 10
#define SALIENCY_THRESHOLD 244
#define SALIENT_MAP_TRAINER_BONUS 3

class EntityManager;

struct DistanceField;

typedef enum __attribute__((__packed__)) {
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

    // Maps further away from the center of the map are use more iterations during the
    // blending pass. Since the blending process favours the proliferation of foliage,
    // an increased number of iterations will lead to a more "overgrown" forest. This
    // has the effect of both impeding traversal and creating more threats against the
    // player.
    float overgrowth;

    // The manhattan distance from the origin, with an extra perturbation on top.
    // Determines the power level of Pokemons encountered on this map.
    float menaceLevel;

    // The chance that a Pokemon will spawn whenever a player walks through a tall grass
    // tile. Also inversely proportional to the number of trainers that will spawn on the
    // map upon first entry.
    // Every time the player defeats or captures a Pokemon, this chance decays slightly.
    float wildernessLevel;

    int numOpponents;

    // A randomly generated name
    std::string name;

    Tile tileset[MAP_HEIGHT][MAP_WIDTH] {};
    DistanceField* memoizedDistanceFields[DISTANCE_FIELD_MEMOIZATION_SIZE] {};
    EntityManager* entityManager;

    MapEntryProps generateTerrain(int worldSeed);

private:
    bool placeRoad(int x, int y, uint8_t edgeBitmask);

    void placeChunk(TileType type, int x, int y, int sizeX, int sizeY);

    void placeKernelChunk(TileType type, int x, int y, float kernelRadius);

    void applyBiomeBlending(int tx, int ty, TileType biome);
};


// Checks whether the position is inside the map, including the borders.
bool isInsideMapBounds(int x, int y);

// Checks whether the position is within the map borders, i.e. excluding the borders and the gate.
bool isInsideMapBorders(int x, int y);

float getMenaceLevel(int globalX, int globalY, float eccentricity, int worldSeed);

#endif
