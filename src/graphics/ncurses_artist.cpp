#include "graphics/ncurses_artist.hpp"
#include "graphics/artist.hpp"

#include <ncurses.h>

void initializeExtendedColors() {
    start_color();

    init_pair(1, 154, 0); // lime
    init_pair(2, 245, 0); // light gray
    init_pair(3, 248, 0); // dark gray
    init_pair(4, 172, 0); // brown
    init_pair(5, 208, 0); // light gray
    init_pair(6, 76, 0);  // deep green
    init_pair(7, 25, 0);  // sky blue
    init_pair(8, 28, 0);  // dark green
    init_pair(9, 165, 0); // purple
    init_pair(10, 196, 0); // full red
    init_pair(11, 216, 0); // light pink
    init_pair(12, 16, 196); // black, deep red bg
}

// @formatter:off
// NOLINTBEGIN
int getTileColorPair(TileType type) {
    switch (type) {
        case FLAT:              return 1;

        case BOULDER:           return 2;

        case BORDER:            return 3;
        case MOUNTAIN:          return 3;

        case GATE:              return 4;
        case ROAD:              return 4;

        case BOULDER_ROAD:      return 5;

        case TALL_GRASS:        return 6;

        case WATER:             return 7;

        case TREE:              return 8;

        case POKECENTER:        return 9;

        case POKEMART:          return 10;

        case JOULDER:           return 11;
        default: return -1;
    }
}
// @formatter:on
// NOLINTEND

void ncursesPrintMap(Map* map, bool isMapColored, int top, int left) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &map->tileset[y][x];
            char tileChar = tileToChar(tile);
            if (isMapColored) {
                int pair = COLOR_PAIR(getTileColorPair(tile->type));
                attron(pair);
                mvaddch(top + y, left + x, tileChar);
                attroff(pair);
            } else {
                mvaddch(top + y, left + x, tileChar);
            }
        }
    }
}
