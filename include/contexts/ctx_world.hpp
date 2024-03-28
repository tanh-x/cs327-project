#ifndef CTX_WORLD_CONTEXT_H
#define CTX_WORLD_CONTEXT_H

#include "core/game.hpp"

class WorldContext : public AbstractContext {
public:
    // Instantiates the root context. Called once when initializing the game.
    WorldContext();

    void start() override {};
};

bool worldContextInputHandler(int key);

EncounterScenario tryPlayerMovementInput(Player* player, int key);

#endif
