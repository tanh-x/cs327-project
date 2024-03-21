#include "contexts/components/elements.h"

void windowTitle(Context* context, char* title) {
    WINDOW* window = context->window;
    mvwprintw(window, 0, 2, "[ %s ]", title);
}