#ifndef CONTEXT_H
#define CONTEXT_H
// Contexts are different stages of the game. The functionality of the game is different depending on whether
// we're in normal gameplay, a battle, or navigating a menu

#include "../core/game.h"
#include "context_type.h"

// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in core objects and the key from getch() to carry out the
// correct action with respect to the current game context; returning whether the input was caught. If not, the key
// will be propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(GameManager* game, GameOptions* options, int key);

#endif
