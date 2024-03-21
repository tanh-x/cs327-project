#ifndef NCURSES_ARTIST_H
#define NCURSES_ARTIST_H

#include <ncurses.h>
#include "world/mapbuilder.h"

void initializeExtendedColors();

void ncursesPrintMap(Map* map, bool isMapColored, int top, int left);

#endif
