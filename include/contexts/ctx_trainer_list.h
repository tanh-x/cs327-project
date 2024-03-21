#ifndef CTX_TRAINER_LIST_H
#define CTX_TRAINER_LIST_H

#include <ncurses.h>
#include "core/game.h"

#define TRAINER_LIST_WINDOW_WIDTH 36
#define TRAINER_LIST_WINDOW_HEIGHT 12

void startTrainerListWindow();

void trainerListEntry(WINDOW* window, WINDOW* parentWindow);

void disposeTrainerListWindow(WINDOW* window, WINDOW* parentWindow);

bool trainerListInputHandler(int key);

#endif
