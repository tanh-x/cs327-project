#include <ncurses.h>
#include "utils/mathematics.h"

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

void expandWindowVertical(Rect2D dimensions, int intervalMicros);

void battleTransition(int intervalMicros);

#endif
