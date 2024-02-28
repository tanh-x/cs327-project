#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "world/world.h"
#include "entity/event.h"

typedef struct EntityManager EntityManager;

#define CMD_MAX_LENGTH 64

// Stores pointers to core components of the game, as well as other information
// Is often passed into various functions for most core game functionality
typedef struct GameManager {
    World* world;
    Player* player;
    EntityManager* entManager;
    int time;
} GameManager;

// Stores the game options, specified via arguments passed by the user
// * doColoring: Whether to color the map when printing to stdout
// * numTrainers: The number of trainers (NPCs) to spawn on every map load
// * frameTime: The number of microseconds to wait between drawing each frame
typedef struct {
    bool doColoring;
    int numTrainers;
    int frameTimeMicros;
} GameOptions;

// The main game loop
void update(GameManager* game, GameOptions* options);

// (Re)draws the frame
void draw(GameManager* game, GameOptions* options);

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(GameManager* game, MapEntryProps* entryProps, GameOptions* options);

// Moves the player to an adjacent map
// Returns the pointer to that map
Map* moveInWorldDirection(GameManager* game, char cmd, MapEntryProps* entryProps);

// Moves the player to a map at the specified parameters (globalX, globalY)
// Returns the pointer to that map
Map* moveToMap(GameManager* game, int globalX, int globalY, MapEntryProps* entryProps);

// Game loop for assignment 1.03 and before
void update_old(GameManager* game, GameOptions* options);

#endif
