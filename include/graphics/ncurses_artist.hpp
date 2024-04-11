#ifndef NCURSES_ARTIST_H
#define NCURSES_ARTIST_H

#include <ncurses.h>
#include "world/mapbuilder.hpp"

#define RDYLGN10_PALETTE_OFFSET 100
#define RDYLGN10_PALETTE_COUNT 20
#define PRGN10_PALETTE_OFFSET 120
#define PRGN10_PALETTE_COUNT 20

void initializeExtendedColors();

void ncursesPrintMap(Map* map, bool isMapColored, int top, int left);

#endif
