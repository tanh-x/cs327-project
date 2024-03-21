#ifndef TRAINER_LIST_H
#define TRAINER_LIST_H

#include <ncurses.h>
#include "core/game.h"

#define TRAINER_LIST_WINDOW_WIDTH 36
#define TRAINER_LIST_WINDOW_HEIGHT 12

void startTrainerListWindow(GameManager* game, WINDOW* parentWindow);

void trainerListEntry(GameManager* game, WINDOW* window, WINDOW* parentWindow);

void disposeTrainerListWindow(WINDOW* window, WINDOW* parentWindow);

bool trainerListInputHandler(GameManager* game, GameOptions* options, int key);

#endif
