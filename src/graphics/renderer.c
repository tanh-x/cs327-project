#include "graphics/renderer.h"
#include "graphics/artist.h"
#include <ncurses.h>

#define MAP_LINE_OFFSET 1

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
//    clear();

    // First line
    mvprintw(0, 0, "Event time: %d", game->time);

    // Draw the map
    char mapStr[MAP_HEIGHT * (MAP_WIDTH + 1) + 1];
    worldToString(game, mapStr);
    mvprintw(MAP_LINE_OFFSET, 0, "%s", mapStr);

    // Draw entities
    ArrayList* entities = game->entManager->entities;
    for (int i = 0; i < entities->size; i++) {
        Entity* ent = entities->array[i];
        mvaddch(MAP_LINE_OFFSET + ent->mapY, ent->mapX, entityToChar(ent));
    }

    // Last two lines
    mvprintw(MAP_HEIGHT + MAP_LINE_OFFSET, 0, "Map position: (%d, %d)", game->player->globalX, game->player->globalY);
    mvprintw(MAP_HEIGHT + MAP_LINE_OFFSET + 1, 0, "PC position: (%d, %d)", game->player->mapX, game->player->mapY);

    refresh();
}