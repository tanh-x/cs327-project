#include "context/components/elements.hpp"

void windowTitle(AbstractContext* context, const char* title, int x) {
    WINDOW* window = context->window;
    mvwprintw(window, 0, x, "[ %s ]", title);
}

void horizontalSeparator(AbstractContext* context, int y) {
    WINDOW* window = context->window;

    // Draw the separator
    wmove(window, y, 0);

    // ACS_LTEE is '├'
    waddch(window, ACS_LTEE);
    for (int i = 0; i < context->dimensions.width - 2; i++) {
        // ACS_HLINE is '─'
        waddch(window, ACS_HLINE);
    }

    // ACS_RTEE is '┤'
    waddch(window, ACS_RTEE);

    // Refresh the window
    wrefresh(window);
}