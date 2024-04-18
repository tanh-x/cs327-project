#ifndef NCURSES_ARTIST_H
#define NCURSES_ARTIST_H

#include <ncurses.h>
#include "world/mapbuilder.hpp"

#define GISTNCAR_PALETTE_OFFSET 100
#define GISTNCAR_PALETTE_COUNT 20
#define PRGN10_PALETTE_OFFSET 120
#define PRGN10_PALETTE_COUNT 20
#define RDYLGN10_PALETTE_OFFSET 140
#define RDYLGN10_PALETTE_COUNT 15
#define PICO8_PALETTE_OFFSET 160
#define PICO8_PALETTE_COUNT 17

void initializeExtendedColors();

void ncursesPrintMap(Map* map, bool isMapColored, int top, int left);

#endif
