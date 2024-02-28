#ifndef GAME_H
#define GAME_H
// This file has components that drive the game, or otherwise is a very centerpiece part of the project.

#include "player.h"
#include "world/world.h"
#include "entity/event.h"

// Forward declarations
typedef struct EntityManager EntityManager;

// Maximum command length (assignment 1.03 and prior)
#define CMD_MAX_LENGTH 64


// Singleton object that stores pointers to core components of the game, as well as other information.
// This is often passed into various functions for most core game functionality.
typedef struct GameManager {
    World* world;
    Player* player;
    EntityManager* entManager;
    int time;
} GameManager;


// Stores the game options, specified via arguments passed by the user.
typedef struct {
    // Whether to color the map when printing to stdout.
    bool doColoring;

    // The number of trainers (NPCs) to spawn on every map load.
    int numTrainers;

    // The number of microseconds to wait between drawing each frame.
    int frameTimeMicros;
} GameOptions;


// The main game loop.
void update(GameManager* game, GameOptions* options);

// (Re)draws the frame.
void draw(GameManager* game, GameOptions* options);

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(GameManager* game, MapEntryProps* entryProps, GameOptions* options);

// Moves the player to an adjacent map.
// Returns the pointer to that map
Map* moveInWorldDirection(GameManager* game, char cmd, MapEntryProps* entryProps);

// Moves the player to a map at the specified parameters (globalX, globalY).
// Returns the pointer to that map
Map* moveToMap(GameManager* game, int globalX, int globalY, MapEntryProps* entryProps);

// Game loop for assignment 1.03 and before.
__attribute__((unused)) void update_old(GameManager* game, GameOptions* options);

#endif
