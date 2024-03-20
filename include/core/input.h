#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "game.h"

// Returns whether the input was actually handled or not
bool handlePlayerInput(GameManager* game, GameOptions* options);

#endif