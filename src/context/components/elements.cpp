#include <algorithm>
#include <cmath>
#include "context/components/elements.hpp"

void windowTitle(AbstractContext* context, const char* title, int x) {
    WINDOW* window = context->window;
    mvwprintw(window, 0, x, "[ %s ]", title);
}

void horizontalSeparator(AbstractContext* context, int x, int y, int width) {
    WINDOW* window = context->window;

    // Draw the separator
    wmove(window, y, x);

    // ACS_LTEE is '├'
    waddch(window, ACS_LTEE);
    for (int i = 0; i < width - 2; i++) {
        // ACS_HLINE is '─'
        waddch(window, ACS_HLINE);
    }

    // ACS_RTEE is '┤'
    waddch(window, ACS_RTEE);

    // Refresh the window
    wrefresh(window);
}

void verticalSeparator(AbstractContext* context, int x, int y, int height) {
    WINDOW* window = context->window;

    // Draw the separator
    wmove(window, y, x);

    // ACS_TTEE is '┬'
    waddch(window, ACS_TTEE);
    for (int i = 0; i < height - 2; i++) {
        // Move cursor down one line
        wmove(window, y + i + 1, x);

        // ACS_VLINE is '│'
        waddch(window, ACS_VLINE);
    }

    // Move cursor to the last position
    wmove(window, y + height - 1, x);

    // ACS_BTEE is '┴'
    waddch(window, ACS_BTEE);

    // Refresh the window
    wrefresh(window);
}


void spaces(AbstractContext* context, int x, int y, int width) {
    WINDOW* window = context->window;

    wmove(window, y, x);
    for (int i = 0; i < width; i++) {
        waddch(window, ' ');
    }

    // Refresh the window
    wrefresh(window);
}


void sequentialColoredBar(
    AbstractContext* context, int x, int y,
    int width, float value,
    int cmapPaletteOffset, int numPaletteColors
) {
    WINDOW* window = context->window;

    // Calculate the width of each color segment
    float segmentWidth = float(width) / float(numPaletteColors);
    int colorIndex;

    wmove(window, y, x);
    auto threshold = static_cast<int>(value);
    for (int i = 0; i < width; i++) {
        // Determine which color pair to use based on the current position
        colorIndex = clamp(std::floor(float(i) / segmentWidth), 0, numPaletteColors - 1);

        if (i <= threshold) {
            wattron(window, COLOR_PAIR(cmapPaletteOffset + colorIndex));
            waddch(window, '#');
            wattroff(window, COLOR_PAIR(cmapPaletteOffset + colorIndex));
        } else {
            wattron(window, COLOR_PAIR(32));
            waddch(window, '-');
            wattroff(window, COLOR_PAIR(31));
        }
    }

    wrefresh(window);
}


void drawBox(
    WINDOW* window,
    int x, int y, int width, int height,
    chtype ls, chtype rs,
    chtype ts, chtype bs,
    chtype tl, chtype tr,
    chtype bl, chtype br
) {
    // Draw top and bottom horizontal lines
    mvwhline(window, y, x + 1, ts, width - 2);
    mvwhline(window, y + height - 1, x + 1, bs, width - 2);

    // Draw left and right vertical lines
    mvwvline(window, y + 1, x, ls, height - 2);
    mvwvline(window, y + 1, x + width - 1, rs, height - 2);

    // Place corners
    mvwaddch(window, y, x, tl);
    mvwaddch(window, y, x + width - 1, tr);
    mvwaddch(window, y + height - 1, x, bl);
    mvwaddch(window, y + height - 1, x + width - 1, br);
}
