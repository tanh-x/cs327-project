#ifndef GAME_H
#define GAME_H
// This file has components that drive the game, or otherwise is a very centerpiece part of the project.

#include <stdbool.h>
#include "game_manager.h"

// Forward declarations
typedef struct EntityManager EntityManager;

// The main game loop.
void gameLoop();

// Set up the current map (as determined by player global position) for gameplay.
// Must be called whenever the map changes
void setupGameOnMapLoad(MapEntryProps* entryProps);

// Moves the player to an adjacent map.
// Returns the pointer to that map
Map* moveInWorldDirection(char cmd, MapEntryProps* entryProps);

// Moves the player to a map at the specified parameters (globalX, globalY).
// Returns the pointer to that map
Map* moveToMap(int globalX, int globalY, MapEntryProps* entryProps);

#endif
