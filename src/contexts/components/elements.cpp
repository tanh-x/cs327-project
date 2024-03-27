#include "contexts/components/elements.hpp"

void windowTitle(AbstractContext* context, const char* title) {
    WINDOW* window = context->window;
    mvwprintw(window, 0, 2, "[ %s ]", title);
}