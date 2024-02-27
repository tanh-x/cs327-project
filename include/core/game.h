#include "player.h"
#include "world/world.h"

#ifndef GAME_H
#define GAME_H

#define CMD_MAX_LENGTH 64

typedef struct {
    World *world;
    Player *player;
    int time;
} GameManager;

typedef struct {
    bool doColoring;
    int numTrainers;
} GameOptions;

void printGameState(GameManager *game);

Map *moveInWorldDirection(GameManager *game, char cmd, MapEntryProps *entryProps);

Map *moveToMap(GameManager *game, int globalX, int globalY, MapEntryProps *entryProps);

void setupGameOnMapLoad(GameManager *game, MapEntryProps *entryProps);

void update(GameManager *game, GameOptions options);

#endif
