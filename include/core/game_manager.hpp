#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <unordered_map>
#include "world/world.hpp"
#include "player.hpp"
#include "game.hpp"
#include "context/abstract_context.hpp"
#include "entities/entity_manager.hpp"
#include "deserialization/pokemon_data.hpp"
#include "database.hpp"

// Stores the game options, specified via arguments passed by the user.
typedef struct {
    // Whether to color the map when printing to stdout.
    bool doColoring;
    // The number of trainers (NPCs) to spawnNpc on every map load.
    int numTrainers;
    // The number of microseconds to wait between drawing each frame.
    int frameTimeMicros;

    bool doBadApple;
} GameOptions;

// Singleton object that stores pointers to core components of the game, as well as other information.
// This is often passed into various functions for most core game functionality.
typedef struct GameManager {
    World* world;
    Player* player;
    EntityManager* currentEntManager;
    AbstractContext* context;
    PokemonDatabase* database;
    bool quit_game;
} GameManager;

extern GameManager GAME;
extern GameOptions OPTIONS;

#endif
