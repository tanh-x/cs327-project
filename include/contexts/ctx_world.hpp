#ifndef CTX_WORLD_CONTEXT_H
#define CTX_WORLD_CONTEXT_H

#include "core/game.hpp"

bool worldContextInputHandler(int key);

EncounterScenario tryPlayerMovementInput(Player* player, int key);

#endif
