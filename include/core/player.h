#ifndef PLAYER_H
#define PLAYER_H
// This file holds functionalities related to the player

#include "entity/entities.h"
#include "world/mapbuilder.h"

#define PLAYER_SPECIAL_ACTION_IDLE_TIME 14
#define PLAYER_REST_TIME 8

// Holds the data related to various aspects of the player
typedef struct {
    int globalX;
    int globalY;
    int mapX;
    int mapY;

    Entity* currentEntity;
} Player;


// Every time the player moves, or an entity moves upon the player, one of a few outcomes can happen
typedef enum __attribute__ ((__packed__)) {
    INVALID,
    STANDARD,
    UNCROSSABLE_TERRAIN,
    ENTITY_ENCOUNTER
} PlayerEncounterScenario;


// Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE or out of bounds.
PlayerEncounterScenario attemptMovePlayer(int dx, int dy);

void dispatchPlayerEncounter(PlayerEncounterScenario scenario);

#endif
