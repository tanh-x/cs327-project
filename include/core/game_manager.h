#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "world/world.h"
#include "player.h"
#include "game.h"
#include "contexts/context_type.h"

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


#endif
