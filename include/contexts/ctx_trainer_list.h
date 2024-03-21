#ifndef CTX_TRAINER_LIST_H
#define CTX_TRAINER_LIST_H

#include <ncurses.h>
#include "core/game.h"

#define TRAINER_LIST_WINDOW_WIDTH 48
#define TRAINER_LIST_WINDOW_HEIGHT 12

void startTrainerListWindow();

void trainerListEntry();

bool trainerListInputHandler(int key);

#endif
