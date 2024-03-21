#include <stdbool.h>
#include "world/mapbuilder.h"
#include "core/game.h"

#ifndef ARTIST_H
#define ARTIST_H

#define CLEAR_SCREEN "\033[2J\033[H"
#define CLEAR_PREV_LINE "\033[A\r\033[K"

void prettyPrint(const char *str, bool isMapColored);

void worldToString(char *str);

char entityToChar(Entity *entity);

char tileToChar(Tile *tile);

#endif