#include "graphics/ncurses_artist.hpp"
#include "graphics/artist.hpp"

#include <ncurses.h>

void initializeExtendedColors() {
    start_color();

    // Original dark themed color scheme
//    init_pair(1, 154, 0); // lime
//    init_pair(2, 245, 0); // light gray
//    init_pair(3, 248, 0); // dark gray
//    init_pair(4, 172, 0); // brown
//    init_pair(5, 208, 0); // light gray
//    init_pair(6, 76, 0);  // deep green
//    init_pair(7, 25, 0);  // sky blue
//    init_pair(8, 28, 0);  // dark green
//    init_pair(9, 165, 0); // purple
//    init_pair(10, 196, 0); // full red
//    init_pair(11, 216, 0); // light pink
//    init_pair(12, 16, 196); // black, deep red bg

    // Inverted color scheme
    init_pair(1, 0, 112); // lime
    init_pair(2, 0, 245); // light gray
    init_pair(3, 0, 248); // dark gray
    init_pair(4, 0, 172); // brown
    init_pair(5, 0, 208); // light gray
    init_pair(6, 0, 70);  // deep green
    init_pair(7, 0, 25);  // sky blue
    init_pair(8, 0, 28);  // dark green
    init_pair(9, 0, 165); // purple
    init_pair(10, 0, 196); // full red
    init_pair(11, 0, 216); // light pink
    init_pair(12, 196, 16); // black, deep red bg

    init_pair(21, 221, COLOR_BLACK);  // light yellow
    init_pair(22, 45, COLOR_BLACK);  // light blue

    // Various shades of gray, for drawing the world map
    init_pair(30, 236, COLOR_BLACK);
    init_pair(31, 237, COLOR_BLACK);
    init_pair(32, 238, COLOR_BLACK);
    init_pair(33, 239, COLOR_BLACK);
    init_pair(34, 240, COLOR_BLACK);
    init_pair(35, 241, COLOR_BLACK);
    init_pair(36, 242, COLOR_BLACK);
    init_pair(37, 243, COLOR_BLACK);
    init_pair(38, 244, COLOR_BLACK);
    init_pair(39, 239, COLOR_BLACK);
    init_pair(40, 238, COLOR_BLACK);
    init_pair(41, 245, COLOR_BLACK);
    init_pair(42, 238, COLOR_BLACK);

    // Sequential color maps for drawing numerical bars

    // Derived from the "gist_ncar" color scheme
    // From matplotlib, licensed under CC0 (public domain).
    // The original palette can be found at
    // https://matplotlib.org/stable/users/explain/colors/colormaps.html#miscellaneous
    const short gistNcarColors[] = {
        40, 46, 82, 192, 184,
        178, 208, 202, 196, 197,
        199, 200, 201, 201, 129,
        129, 126, 126, 90, 239
    };

    for (int i = 0; i < GISTNCAR_PALETTE_COUNT; i++) {
        init_extended_pair(GISTNCAR_PALETTE_OFFSET + i, gistNcarColors[i], COLOR_BLACK);
    }

    // Derived from the "PRGn_10" color scheme
    // From colorbrewer, licensed under CC0 (public domain); distributed by ColorSchemes.jl
    // The original palette is
    // #40004b, #762a83, #9970ab, #c2a5cf, #e7d4e8,
    // #d9f0d3, #a6dba0, #5aae61, #1b7837, #00441b
    // It can also be found at
    // https://docs.juliaplots.org/latest/generated/colorschemes#colorbrewer
    const short prGn10Colors[] = {
        91, 97, 103, 146, 194,
        192, 155, 119, 118, 82,
        46, 40, 34, 28, 22,
        22, 22, 23, 23, 23,
    };

    for (int i = 0; i < PRGN10_PALETTE_COUNT; i++) {
        init_extended_pair(PRGN10_PALETTE_OFFSET + i, prGn10Colors[i], COLOR_BLACK);
    }
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
