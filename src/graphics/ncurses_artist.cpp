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

    // Derived from the "RdYlGn_10" color scheme
    // From colorbrew, licensed under CC0 (public domain); distributed by ColorSchemes.jl
    // The original palette is
    // #a50026, #d73027, #f46d43, #fdae61, #fee08b,
    // #d9ef8b, #a6d96a, #66bd63, #1a9850, #006837
    init_pair(RDYLGN10_PALETTE_OFFSET + 0,  40, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 1,  46, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 2,  82, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 3,  192, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 4,  184, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 5,  178, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 6,  208, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 7,  202, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 8,  196, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 9,  197, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 10, 199, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 11, 200, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 12, 201, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 13, 201, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 14, 129, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 15, 129, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 16, 126, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 17, 126, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 18, 90, COLOR_BLACK);
    init_pair(RDYLGN10_PALETTE_OFFSET + 19, 239, COLOR_BLACK);

    // Derived from the "PRGn_10" color scheme
    // From colorbrew, licensed under CC0 (public domain); distributed by ColorSchemes.jl
    // The original palette is
    // #40004b, #762a83, #9970ab, #c2a5cf, #e7d4e8,
    // #d9f0d3, #a6dba0, #5aae61, #1b7837, #00441b
    init_pair(PRGN10_PALETTE_OFFSET + 0, 91, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 1, 97, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 2, 103, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 3, 146, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 4, 194, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 5, 192, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 6, 155, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 7, 119, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 8, 118, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 9, 82, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 10, 46, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 11, 40, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 12, 34, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 13, 28, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 14, 22, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 15, 22, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 16, 22, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 17, 23, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 18, 23, COLOR_BLACK);
    init_pair(PRGN10_PALETTE_OFFSET + 19, 23, COLOR_BLACK);

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
