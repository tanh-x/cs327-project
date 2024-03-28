#include <cstdlib>
#include "contexts/abstract_context.hpp"
#include "contexts/ctx_world.hpp"
#include "contexts/ctx_trainer_list.hpp"

AbstractContext::AbstractContext(ContextType type, AbstractContext* parent, Rect2D dimensions) {
    this->window = nullptr;
    this->type = type;
    this->parent = parent;
    this->dimensions = dimensions;
}

// Disposes of the context, which is assumed to be the current context, and restores the terminal to the parent window.
// Additionally, restores the
void AbstractContext::returnToParentContext() {
    // Do not call this method on a context that is not the current context, or if it's the root context
    if (GAME.context != this || parent == nullptr) exit(1);

    // Destroy the context and restore the parent
    delwin(window);
    wrefresh(parent->window);
    GAME.context = parent;
    free(this);
}

// Constructs a basic popup window, and sets the current window to it
WINDOW* AbstractContext::constructWindow() {
    WINDOW* newWindow = newwin(dimensions.height, dimensions.width, dimensions.y, dimensions.x);

    // Initialize window stuff
    keypad(newWindow, true);

    // Initialize styling
    box(newWindow, 0, 0);

    // Set the context's pointer to it and return it as well
    this->window = newWindow;
    return newWindow;
}

void AbstractContext::refreshContext() const {
    wrefresh(window);
}

void AbstractContext::start() {
    GAME.context = this;
    refreshContext();
}

bool emptyInputHandler(int key) {
    return false;
}

// TODO: old C code, refactor this into polymorphism
// Given the current context type, returns the pointer to a 3-ary function, called the input handler, corresponding
// to the current context type. The input handler takes in the key from getch() to carry out the correct action
// with respect to the current game context; returning whether the input was caught. If not, the key will be
// propagated to the next input handler.
bool (* dispatchContextInputHandler(ContextType type))(int key) {
    switch (type) {
        case ContextType::WORLD_CONTEXT: return worldContextInputHandler;
        case ContextType::TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return emptyInputHandler;
    }
}
