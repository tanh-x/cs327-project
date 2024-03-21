#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "world/world.h"
#include "player.h"
#include "game.h"
#include "contexts/context_type.h"

// Stores the game options, specified via arguments passed by the user.
typedef struct {
    // Whether to color the map when printing to stdout.
    bool doColoring;
    // The number of trainers (NPCs) to spawn on every map load.
    int numTrainers;
    // The number of microseconds to wait between drawing each frame.
    int frameTimeMicros;
} GameOptions;

// Singleton object that stores pointers to core components of the game, as well as other information.
// This is often passed into various functions for most core game functionality.
typedef struct GameManager {
    World* world;
    Player* player;
    EntityManager* entManager;
    ContextType context;
    int time;
    bool quit_game;
} GameManager;

extern GameManager GAME;
extern GameOptions OPTIONS;

#endif
