#include "graphics/renderer.h"
#include "graphics/artist.h"
#include <ncurses.h>

// Initializes the ncurses library
void initializeRenderer(bool enableColor) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (enableColor && has_colors()) start_color();
}

void cleanUpRenderer() {
    endwin();
}

void renderGameUpdate(GameManager* game, GameOptions* options) {
    clear();

    // First line
    mvprintw(0, 0, "Event time: %d", game->time);

    // Draw the map
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(game, mapStr);
    mvprintw(1, 0, "%s", mapStr);


    // Last two lines
    mvprintw(MAP_HEIGHT + 1, 0, "Map position: (%d, %d)", game->player->globalX, game->player->globalY);
    mvprintw(MAP_HEIGHT + 2, 0, "PC position: (%d, %d)", game->player->mapX, game->player->mapY);

    refresh();
}