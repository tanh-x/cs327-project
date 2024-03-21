#include <ncurses.h>
#include "contexts/ctx_building.h"

void enterPlaceholderBuilding(TileType type) {
    WINDOW* parentWindow = stdscr;
    int width = WINDOW_WIDTH - 4;
    int height = WINDOW_HEIGHT - 4;

    // Find the center of the window
    int top = (WINDOW_HEIGHT - height) / 2;
    int left = (WINDOW_WIDTH - width) / 2;

    // Instantiate new window
    WINDOW* window = newwin(height, width, top, left);

    // Draw a box around the window
    box(window, 0, 0);
    keypad(window, TRUE);

    if (type == POKEMART) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMART INTERFACE");
    else if (type == POKECENTER) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMON CENTER INTERFACE");

    // We're done with initialization
    GAME.context = BUILDING_CONTEXT;
    wrefresh(window);

    // Only accepts ESC to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == '<' || ch == '[') break;
    }

    // If we got here, ESC has been pressed
    delwin(window);
    wrefresh(parentWindow);
    GAME.context = WORLD_CONTEXT;
}
