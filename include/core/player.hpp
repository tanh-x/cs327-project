#ifndef PLAYER_H
#define PLAYER_H

// This file holds functionalities related to the player

#include "entity/entities.hpp"
#include "world/mapbuilder.hpp"

constexpr int PLAYER_SPECIAL_ACTION_IDLE_TIME = 5;
constexpr int PLAYER_REST_TIME = 8;

enum class EncounterScenario {
    INVALID,
    STANDARD,
    UNCROSSABLE_TERRAIN,
    BORDER_APPROACH,
    ENTITY_ENCOUNTER
};

class Player {
public:
    int globalX;
    int globalY;
    int mapX;
    int mapY;

    Entity* currentEntity;

    Player() : globalX(0), globalY(0), mapX(0), mapY(0), currentEntity(nullptr) {}

    // Tries to move the player along the specified direction, which might fail if the tile is UNCROSSABLE
    // or out of bounds.
    EncounterScenario attemptMove(int dx, int dy);

    void possessEntity(PlayerVessel* entity);
};

void dispatchPlayerEncounter(EncounterScenario scenario);

#endif
