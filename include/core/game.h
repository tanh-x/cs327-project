#include "player.h"
#include "world/world.h"

#ifndef GAME_H
#define GAME_H

#define CMD_MAX_LENGTH 64

typedef struct {
    World *world;
    Player *player;
} GameManager;

typedef struct {
    bool doColoring;
} GameOptions;

void printGameState(GameManager *game);

Map *moveInWorldDirection(GameManager *game, char cmd);

Map *moveToMap(GameManager *game, int globalX, int globalY);

void update(GameManager *game, GameOptions options);

#endif
