#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "context/abstract_context.hpp"

#define ASPECT 2
#define SPRITE_HEIGHT 16

void windowTitle(AbstractContext* context, const char* title, int x);

void horizontalSeparator(AbstractContext* context, int x, int y, int width);

void verticalSeparator(AbstractContext* context, int x, int y, int height);

void spaces(AbstractContext* context, int x, int y, int width);

void sequentialColoredBar(
    AbstractContext* context, int x, int y,
    int width, float value,
    int cmapPaletteOffset, int numPaletteColors,
    bool homogeneous = false,
    chtype filled = '#',
    chtype empty = '-'
);

void drawBox(
    WINDOW* window,
    int x, int y, int width, int height,
    chtype ls = ACS_VLINE, chtype rs = ACS_VLINE,
    chtype ts = ACS_HLINE, chtype bs = ACS_HLINE,
    chtype tl = ACS_ULCORNER, chtype tr = ACS_URCORNER,
    chtype bl = ACS_LLCORNER, chtype br = ACS_LRCORNER
);

void rasterizePokemonSprite(WINDOW* window, int pokemonId, int x, int y, bool flip);


#endif
