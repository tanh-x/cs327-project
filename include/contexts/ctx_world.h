#ifndef WORLD_CONTEXT_H
#define WORLD_CONTEXT_H

#include "core/game.h"

PlayerEncounterScenario tryPlayerMovementInput(GameManager* game, int key);

bool worldContextInputHandler(GameManager* game, GameOptions* options, int key);

#endif
