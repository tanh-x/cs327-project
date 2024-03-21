#include <ncurses.h>
#include "contexts/ctx_building.h"

void enterPlaceholderBuilding(TileType type) {
    // Define dimensions of the new window
    Rect2D windowDimensions;
    windowDimensions.width = WINDOW_WIDTH - 6;
    windowDimensions.height = WINDOW_HEIGHT - 6;

    // Find the center of the parent window
    windowDimensions.x = (WINDOW_WIDTH - windowDimensions.width) / 2;
    windowDimensions.y = (WINDOW_HEIGHT - windowDimensions.height) / 2;

    // Construct and switch to it
    Context* context = constructChildWindowContext(BUILDING_CONTEXT, windowDimensions);
    WINDOW* window = context->window;

    if (type == POKEMART) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMART INTERFACE");
    else if (type == POKECENTER) mvwprintw(window, 1, 1, "PLACEHOLDER POKEMON CENTER INTERFACE");

    // We're done with initialization
    wrefresh(window);

    // Only accepts </[ to exit, no other keys are handled.
    while (true) {
        int ch = getch();
        if (ch == '<' || ch == '[') break;
    }

    // If we got here, </[ has been pressed
    GAME.context = context->parent;
    returnToParentContext(context);
}
