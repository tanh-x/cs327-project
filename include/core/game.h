#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "world/world.h"
#include "entity/event.h"

typedef struct EntityManager EntityManager;


#define CMD_MAX_LENGTH 64

typedef struct GameManager {
    World *world;
    Player *player;
    EntityManager *entManager;
    int time;
} GameManager;

typedef struct {
    bool doColoring;
    int numTrainers;
} GameOptions;

void printGameState(GameManager *game);

Map *moveInWorldDirection(GameManager *game, char cmd, MapEntryProps *entryProps);

Map *moveToMap(GameManager *game, int globalX, int globalY, MapEntryProps *entryProps);

void setupGameOnMapLoad(GameManager *game, MapEntryProps *entryProps, GameOptions *options);

void update(GameManager *game, GameOptions *options);

#endif
