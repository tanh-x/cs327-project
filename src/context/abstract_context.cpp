#include <cstdlib>
#include "context/abstract_context.hpp"
#include "context/ctx_main.hpp"
#include "context/ctx_trainer_list.hpp"

// Contexts are different stages of the game. The functionality of the game is different depending on whether
// we're in normal gameplay, a battle, or navigating a menu. Contexts are invoked by a constructor call, which
// initializes the context. The caller MUST then invoke Context::start() to begin operation of the context window.
// Not calling Context::start() *may* result in undefined behaviour, as there can potentially be side effects that
// have already been made during the constructor call.
AbstractContext::AbstractContext(ContextType type, AbstractContext* parent, Rect2D dimensions) {
    this->window = nullptr;
    this->type = type;
    this->parent = parent;
    this->dimensions = dimensions;
}

// The main loop of the context UI.
// Every start() implementation MUST end with a singular returnToParentContext() call
// When start() returns, the context is guaranteed to have been restored to the parent context. No additional
// handling is necessary
void AbstractContext::start() {
    GAME.context = this;
    refreshContext();
}

// Disposes of the context, which is assumed to be the current context, and restores the parent window.
void AbstractContext::returnToParentContext() {
    // Do not call this method on a context that is not the current context, or if it's the root context
    if (GAME.context != this || parent == nullptr) exit(1);

    // Destroy the context and restore the parent
    delwin(window);
    wrefresh(parent->window);
    clear();
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
        case ContextType::MAIN_CONTEXT: return worldContextInputHandler;
        case ContextType::TRAINER_LIST_CONTEXT: return trainerListInputHandler;
        default: return emptyInputHandler;
    }
}
