#include <stdbool.h>
#include "world/mapbuilder.h"

#ifndef ARTIST_H
#define ARTIST_H

#define CLEAR_SCREEN "\033[2J\033[H"

void prettyPrint(const char *str, bool isMapColored);

void worldToString(Map *map, char *str);

#endif