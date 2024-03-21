#ifndef RENDERER_H
#define RENDERER_H
// This file has functionalities related to graphics rendering with the ncurses library

#include <stdbool.h>
#include "core/game.h"

// Initializes the ncurses library
void initializeRenderer(bool enableColor);

// Call at the end of the program
void cleanUpRenderer();

void renderGameUpdate();

#endif
