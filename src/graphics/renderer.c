#include "graphics/renderer.h"
#include "graphics/artist.h"
#include "graphics/ncurses_artist.h"
#include <ncurses.h>

#define MAP_LINE_OFFSET 1

// Initializes the ncurses library
void initializeRenderer(bool enableColor) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);


    if (enableColor && has_colors()) initializeExtendedColors();
}

void cleanUpRenderer() {
    endwin();
}

void renderGameUpdate() {
//    clear();

    // First line
    mvprintw(0, 0, "Event time: %d", GAME.time);

    // Draw the map
    ncursesPrintMap(GAME.world->current, OPTIONS.doColoring, MAP_LINE_OFFSET, 0);
//    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
//    worldToString(mapStr);
//    ncursesPrintMap(mapStr, OPTIONS.doColoring);
//    mvprintw(MAP_LINE_OFFSET, 0, "%s", mapStr);

    // Draw entities
    ArrayList* entities = GAME.entManager->entities;
    for (int i = 0; i < entities->size; i++) {
        Entity* ent = entities->array[i];
        mvaddch(MAP_LINE_OFFSET + ent->mapY, ent->mapX, entityToChar(ent));
    }

    // Last two lines
    mvprintw(MAP_HEIGHT + MAP_LINE_OFFSET, 0, "Map position: (%d, %d)", GAME.player->globalX, GAME.player->globalY);
    mvprintw(MAP_HEIGHT + MAP_LINE_OFFSET + 1, 0, "PC position: (%d, %d)", GAME.player->mapX, GAME.player->mapY);

    refresh();
}