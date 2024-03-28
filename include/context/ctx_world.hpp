#ifndef CTX_WORLD_H
#define CTX_WORLD_H

#include "core/game.hpp"

class MainContext : public AbstractContext {
public:
    // Instantiates the root context. Called once when initializing the game.
    MainContext();

    void start() override {};
};

bool worldContextInputHandler(int key);

EncounterScenario tryPlayerMovementInput(Player* player, int key);

#endif
